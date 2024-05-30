#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <cmath>
#include <pthread.h>
#include "BMP.cpp"

class Ellipse : public Drawable {
private:
    int xc, yc, r1, r2;
    uint8_t r, g, b;

public:
    Ellipse(int xc, int yc, int r1, int r2, uint8_t r, uint8_t g, uint8_t b)
        : xc(xc), yc(yc), r1(r1), r2(r2), r(r), g(g), b(b) {}

    void draw(BMP &image) const override {
        int x = 0;
        int y = r2;
        int r1_2 = r1 * r1;
        int r2_2 = r2 * r2;
        int fa2 = 4 * r1_2, fb2 = 4 * r2_2;
        int sigma = 2 * r2_2 + r1_2 * (1 - 2 * r2);

        while (r2_2 * x <= r1_2 * y) {
            image.setPixel(xc + x, yc + y, r, g, b);
            image.setPixel(xc - x, yc + y, r, g, b);
            image.setPixel(xc + x, yc - y, r, g, b);
            image.setPixel(xc - x, yc - y, r, g, b);

            if (sigma >= 0) {
                sigma += fa2 * (1 - y);
                y--;
            }
            sigma += r2_2 * ((4 * x) + 6);
            x++;
        }

        x = r1;
        y = 0;
        sigma = 2 * r1_2 + r2_2 * (1 - 2 * r1);

        while (r1_2 * y <= r2_2 * x) {
            image.setPixel(xc + x, yc + y, r, g, b);
            image.setPixel(xc - x, yc + y, r, g, b);
            image.setPixel(xc + x, yc - y, r, g, b);
            image.setPixel(xc - x, yc - y, r, g, b);

            if (sigma >= 0) {
                sigma += fb2 * (1 - x);
                x--;
            }
            sigma += r1_2 * ((4 * y) + 6);
            y++;
        }
    }

    void draw_openmp(BMP &image, int thread_num) const override {
        int r1_2 = r1 * r1;
        int r2_2 = r2 * r2;

        #pragma omp parallel sections num_threads(thread_num)
        {
            #pragma omp section
            {
                int x = 0;
                int y = r2;
                int fa2 = 4 * r1_2, fb2 = 4 * r2_2;
                int sigma = 2 * r2_2 + r1_2 * (1 - 2 * r2);

                while (r2_2 * x <= r1_2 * y) {
                    image.setPixel(xc + x, yc + y, r, g, b);
                    image.setPixel(xc - x, yc + y, r, g, b);
                    image.setPixel(xc + x, yc - y, r, g, b);
                    image.setPixel(xc - x, yc - y, r, g, b);

                    if (sigma >= 0) {
                        sigma += fa2 * (1 - y);
                        y--;
                    }
                    sigma += r2_2 * ((4 * x) + 6);
                    x++;
                }
            }

            #pragma omp section
            {
                int x = r1;
                int y = 0;
                int sigma = 2 * r1_2 + r2_2 * (1 - 2 * r1);

                while (r1_2 * y <= r2_2 * x) {
                    image.setPixel(xc + x, yc + y, r, g, b);
                    image.setPixel(xc - x, yc + y, r, g, b);
                    image.setPixel(xc + x, yc - y, r, g, b);
                    image.setPixel(xc - x, yc - y, r, g, b);

                    if (sigma >= 0) {
                        sigma += 4 * r2_2 * (1 - x);
                        x--;
                    }
                    sigma += r1_2 * ((4 * y) + 6);
                    y++;
                }
            }
        }
    }

    void draw_threadlib(BMP &image, int thread_num) const override {
        int r1_2 = r1 * r1;
        int r2_2 = r2 * r2;

        int segmentLength = r1 / thread_num;
        int remainder = r1 % thread_num;

        struct ThreadData {
            BMP* image;
            int xc, yc;
            int r1_2, r2_2;
            int start_x, end_x;
            uint8_t r, g, b;
        };

        auto threadFunc = [](void* arg) -> void* {
            ThreadData* data = static_cast<ThreadData*>(arg);
            int x = data->start_x;
            int y = data->r2_2 / data->r1_2;
            int fa2 = 4 * data->r1_2, fb2 = 4 * data->r2_2;
            int sigma = 2 * data->r2_2 + data->r1_2 * (1 - 2 * data->r2_2);

            while (x <= data->end_x && data->r2_2 * x <= data->r1_2 * y) {
                data->image->setPixel(data->xc + x, data->yc + y, data->r, data->g, data->b);
                data->image->setPixel(data->xc - x, data->yc + y, data->r, data->g, data->b);
                data->image->setPixel(data->xc + x, data->yc - y, data->r, data->g, data->b);
                data->image->setPixel(data->xc - x, data->yc - y, data->r, data->g, data->b);

                if (sigma >= 0) {
                    sigma += fa2 * (1 - y);
                    y--;
                }
                sigma += data->r2_2 * ((4 * x) + 6);
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
            threadData[i] = { &image, xc, yc, r1_2, r2_2, start_x, end_x, r, g, b };
            pthread_create(&threads[i], nullptr, threadFunc, &threadData[i]);
            start_x = end_x + 1;
        }

        for (int i = 0; i < thread_num; ++i) {
            pthread_join(threads[i], nullptr);
        }

        segmentLength = r2 / thread_num;
        remainder = r2 % thread_num;

        auto threadFuncSecondHalf = [](void* arg) -> void* {
            ThreadData* data = static_cast<ThreadData*>(arg);
            int x = data->r1_2 / data->r2_2;
            int y = data->start_x;
            int fa2 = 4 * data->r1_2, fb2 = 4 * data->r2_2;
            int sigma = 2 * data->r1_2 + data->r2_2 * (1 - 2 * x);

            while (y <= data->end_x && data->r1_2 * y <= data->r2_2 * x) {
                data->image->setPixel(data->xc + x, data->yc + y, data->r, data->g, data->b);
                data->image->setPixel(data->xc - x, data->yc + y, data->r, data->g, data->b);
                data->image->setPixel(data->xc + x, data->yc - y, data->r, data->g, data->b);
                data->image->setPixel(data->xc - x, data->yc - y, data->r, data->g, data->b);

                if (sigma >= 0) {
                    sigma += fb2 * (1 - x);
                    x--;
                }
                sigma += data->r1_2 * ((4 * y) + 6);
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
            threadData[i] = { &image, xc, yc, r1_2, r2_2, start_x, end_x, r, g, b };
            pthread_create(&threads[i], nullptr, threadFuncSecondHalf, &threadData[i]);
            start_x = end_x + 1;
        }

        for (int i = 0; i < thread_num; ++i) {
            pthread_join(threads[i], nullptr);
        }
    }
};

#endif