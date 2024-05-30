#ifndef LINE_H
#define LINE_H

#include <cmath>
#include "BPM.cpp"

class Line : public Drawable {
private:
    int x0, y0, x1, y1;
    uint8_t r, g, b;

public:
    Line(int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b)
        : x0(x0), y0(y0), x1(x1), y1(y1), r(r), g(g), b(b) {}

    void draw(BMP &image) const override {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        int x = x0;
        int y = y0;

        while (true) {
            image.setPixel(x, y, r, g, b);
            if (x == x1 && y == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }

    void draw_openmp(BMP &image, int thread_num) const override {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        int x = x0;
        int y = y0;

        int num_segments = thread_num; // Define the number of segments
        int segment_length = (std::max(dx, dy) + num_segments - 1) / num_segments; // Calculate length of each segment

        #pragma omp parallel for num_threads(thread_num)
        for (int segment = 0; segment < num_segments; ++segment) {
            int segment_start = segment * segment_length;
            int segment_end = std::min((segment + 1) * segment_length, std::max(dx, dy));

            int local_x = x;
            int local_y = y;
            int local_err = err;

            for (int i = segment_start; i < segment_end; ++i) {
                image.setPixel(local_x, local_y, r, g, b);
                int e2 = 2 * local_err;
                if (e2 > -dy) {
                    local_err -= dy;
                    local_x += sx;
                }
                if (e2 < dx) {
                    local_err += dx;
                    local_y += sy;
                }
            }

            // Synchronize starting points for next segment
            #pragma omp critical
            {
                if (local_x == x1 && local_y == y1) break;
                x = local_x;
                y = local_y;
                err = local_err;
            }
        }
    }
};

#endif