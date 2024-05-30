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

        int num_segments = thread_num; // Define the number of segments
        int segment_length_x = (dx + num_segments - 1) / num_segments; // Calculate length of each segment in x direction
        int segment_length_y = (dy + num_segments - 1) / num_segments; // Calculate length of each segment in y direction

        #pragma omp parallel for
        for (int segment = 0; segment < num_segments; ++segment) {
            // Calculate starting point for the segment
            double ratio_x = static_cast<double>(segment * segment_length_x) / dx;
            double ratio_y = static_cast<double>(segment * segment_length_y) / dy;
            double local_x0 = x0 + ratio_x * (x1 - x0);
            double local_y0 = y0 + ratio_y * (y1 - y0);

            // Calculate ending point for the segment
            double ratio_x1 = static_cast<double>((segment + 1) * segment_length_x) / dx;
            double ratio_y1 = static_cast<double>((segment + 1) * segment_length_y) / dy;
            double local_x1 = x0 + ratio_x1 * (x1 - x0);
            double local_y1 = y0 + ratio_y1 * (y1 - y0);

            // Ensure local_x and local_y are integers for pixel positions
            int lx0 = static_cast<int>(round(local_x0));
            int ly0 = static_cast<int>(round(local_y0));
            int lx1 = static_cast<int>(round(local_x1));
            int ly1 = static_cast<int>(round(local_y1));

            // Calculate local error
            int local_dx = std::abs(lx1 - lx0);
            int local_dy = std::abs(ly1 - ly0);
            int local_err = local_dx - local_dy;

            int local_x = lx0;
            int local_y = ly0;

            // Draw the segment
            while (true) {
                image.setPixel(local_x, local_y, r, g, b);
                if (local_x == lx1 && local_y == ly1) break;
                int e2 = 2 * local_err;
                if (e2 > -local_dy) {
                    local_err -= local_dy;
                    local_x += sx;
                }
                if (e2 < local_dx) {
                    local_err += local_dx;
                    local_y += sy;
                }
            }
        }
    }
};

#endif