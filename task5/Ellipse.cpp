#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <cmath>
#include <pthread.h>
#include "BMP.h"

class Ellipse : public Drawable {
private:
    int xc, yc, a, b;
    uint8_t r, g, b;

public:
    Ellipse(int xc, int yc, int a, int b, uint8_t r, uint8_t g, uint8_t b)
        : xc(xc), yc(yc), a(a), b(b), r(r), g(g), b(b) {}

    void draw(BMP &image) const override {
        int x = 0;
        int y = b;
        int a2 = a * a;
        int b2 = b * b;
        int fa2 = 4 * a2, fb2 = 4 * b2;
        int sigma = 2 * b2 + a2 * (1 - 2 * b);

        while (b2 * x <= a2 * y) {
            image.setPixel(xc + x, yc + y, r, g, b);
            image.setPixel(xc - x, yc + y, r, g, b);
            image.setPixel(xc + x, yc - y, r, g, b);
            image.setPixel(xc - x, yc - y, r, g, b);

            if (sigma >= 0) {
                sigma += fa2 * (1 - y);
                y--;
            }
            sigma += b2 * ((4 * x) + 6);
            x++;
        }

        x = a;
        y = 0;
        sigma = 2 * a2 + b2 * (1 - 2 * a);

        while (a2 * y <= b2 * x) {
            image.setPixel(xc + x, yc + y, r, g, b);
            image.setPixel(xc - x, yc + y, r, g, b);
            image.setPixel(xc + x, yc - y, r, g, b);
            image.setPixel(xc - x, yc - y, r, g, b);

            if (sigma >= 0) {
                sigma += fb2 * (1 - x);
                x--;
            }
            sigma += a2 * ((4 * y) + 6);
            y++;
        }
    }

    void draw_openmp(BMP &image, int thread_num) const override {
        int a2 = a * a;
        int b2 = b * b;

        auto draw_half_ellipse = [&](int start_x, int end_x) {
            int x = start_x;
            int y = b;
            int fa2 = 4 * a2, fb2 = 4 * b2;
            int sigma = 2 * b2 + a2 * (1 - 2 * b);

            while (x <= end_x && b2 * x <= a2 * y) {
                image.setPixel(xc + x, yc + y, r, g, b);
                image.setPixel(xc - x, yc + y, r, g, b);
                image.setPixel(xc + x, yc - y, r, g, b);
                image.setPixel(xc - x, yc - y, r, g, b);

                if (sigma >= 0) {
                    sigma += fa2 * (1 - y);
                    y--;
                }
                sigma += b2 * ((4 * x) + 6);
                x++;
            }
        };

        #pragma omp parallel for num_threads(thread_num)
        for (int i = 0; i < thread_num; ++i) {
            int start_x = i * a / thread_num;
            int end_x = (i + 1) * a / thread_num;
            draw_half_ellipse(start_x, end_x);
        }

        auto draw_second_half = [&](int start_y, int end_y) {
            int x = a;
            int y = start_y;
            int sigma = 2 * a2 + b2 * (1 - 2 * a);

            while (y <= end_y && a2 * y <= b2 * x) {
                image.setPixel(xc + x, yc + y, r, g, b);
                image.setPixel(xc - x, yc + y, r, g, b);
                image.setPixel(xc + x, yc - y, r, g, b);
                image.setPixel(xc - x, yc - y, r, g, b);

                if (sigma >= 0) {
                    sigma += fb2 * (1 - x);
                    x--;
                }
                sigma += a2 * ((4 * y) + 6);
                y++;
            }
        };

        #pragma omp parallel for num_threads(thread_num)
        for (int i = 0; i < thread_num; ++i) {
            int start_y = i * b / thread_num;
            int end_y = (i + 1) * b / thread_num;
            draw_second_half(start_y, end_y);
        }
    }

    void draw_threadlib(BMP &image, int thread_num) const override {
        int a2 = a * a;
        int b2 = b * b;

        int segmentLength = a / thread_num;
        int remainder = a % thread_num;

        struct ThreadData {
            BMP* image;
            int xc, yc;
            int a2, b2;
            int start_x, end_x;
            uint8_t r, g, b;
        };

        auto threadFunc = [](void* arg) -> void* {
            ThreadData* data = static_cast<ThreadData*>(arg);
            int x = data->start_x;
            int y = b;
            int fa2 = 4 * data->a2, fb2 = 4 * data->b2;
            int sigma = 2 * data->b2 + data->a2 * (1 - 2 * b);

            while (x <= data->end_x && data->b2 * x <= data->a2 * y) {
                data->image->setPixel(data->xc + x, data->yc + y, data->r, data->g, data->b);
                data->image->setPixel(data->xc - x, data->yc + y, data->r, data->g, data->b);
                data->image->setPixel(data->xc + x, data->yc - y, data->r, data->g, data->b);
                data->image->setPixel(data->xc - x, data->yc - y, data->r, data->g, data->b);

                if (sigma >= 0) {
                    sigma += fa2 * (1 - y);
                    y--;
                }
                sigma += data->b2 * ((4 * x) + 6);
                x++;
            }
            return nullptr;
        };

        std::vector<pthread_t> threads(thread_num);
        std::vector<ThreadData> threadData(thread_num);

        int start_x = 0;
        for (int i = 0; i < thread_num; ++i) {
            int end_x = start_x + segmentLength - 1;
            if (i < remainder) {
                end_x += 1;
            }
            threadData[i] = { &image, xc, yc, a2, b2, start_x, end_x, r, g, b };
            pthread_create(&threads[i], nullptr, threadFunc, &threadData[i]);
            start_x = end_x + 1;
        }

        for (int i = 0; i < thread_num; ++i) {
            pthread_join(threads[i], nullptr);
        }

        segmentLength = b / thread_num;
        remainder = b % thread_num;

        auto threadFuncSecondHalf = [](void* arg) -> void* {
            ThreadData* data = static_cast<ThreadData*>(arg);
            int x = data->a2 / data->b2;
            int y = data->start_x;
            int sigma = 2 * data->a2 + data->b2 * (1 - 2 * x);

            while (y <= data->end_x && data->a2 * y <= data->b2 * x) {
                data->image->setPixel(data->xc + x, data->yc + y, data->r, data->g, data->b);
                data->image->setPixel(data->xc - x, data->yc + y, data->r, data->g, data->b);
                data->image->setPixel(data->xc + x, data->yc - y, data->r, data->g, data->b);
                data->image->setPixel(data->xc - x, data->yc - y, data->r, data->g, data->b);

                if (sigma >= 0) {
                    sigma += fb2 * (1 - x);
                    x--;
                }
                sigma += data->a2 * ((4 * y) + 6);
                y++;
            }
            return nullptr;
        };

        start_x = 0;
        for (int i = 0; i < thread_num; ++i) {
            int end_x = start_x + segmentLength - 1;
            if (i < remainder) {
                end_x += 1;
            }
            threadData[i] = { &image, xc, yc, a2, b2, start_x, end_x, r, g, b };
            pthread_create(&threads[i], nullptr, threadFuncSecondHalf, &threadData[i]);
            start_x = end_x + 1;
        }

        for (int i = 0; i < thread_num; ++i) {
            pthread_join(threads[i], nullptr);
        }
    }
};

#endif