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
};

#endif