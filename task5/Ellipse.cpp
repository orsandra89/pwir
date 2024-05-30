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

    struct ThreadData {
        BMP* image;
        int xc, yc;
        int r1, r2;
        int start, end;
        uint8_t r, g, b;
    };

    static void* threadFunc(void* arg) {
        ThreadData* data = static_cast<ThreadData*>(arg);
        int xc = data->xc;
        int yc = data->yc;
        int r1 = data->r1;
        int r2 = data->r2;
        uint8_t r = data->r;
        uint8_t g = data->g;
        uint8_t b = data->b;
        BMP* image = data->image;

        for (int angle = data->start; angle <= data->end; ++angle) {
            double theta = angle * M_PI / 180.0;
            int x = static_cast<int>(r1 * cos(theta));
            int y = static_cast<int>(r2 * sin(theta));

            image->setPixel(xc + x, yc + y, r, g, b);
            image->setPixel(xc - x, yc + y, r, g, b);
            image->setPixel(xc + x, yc - y, r, g, b);
            image->setPixel(xc - x, yc - y, r, g, b);
        }
        return nullptr;
    }

    void draw_threadlib(BMP &image, int thread_num) const override {
        int segmentLength = 360 / thread_num;
        int remainder = 360 % thread_num;

        std::vector<pthread_t> threads(thread_num);
        std::vector<ThreadData> threadData(thread_num);

        int start = 0;
        for (int i = 0; i < thread_num; ++i) {
            int end = start + segmentLength - 1;
            if (i < remainder) {
                end += 1;
            }
            threadData[i] = { &image, xc, yc, r1, r2, start, end, r, g, b };
            pthread_create(&threads[i], nullptr, threadFunc, &threadData[i]);
            start = end + 1;
        }

        for (int i = 0; i < thread_num; ++i) {
            pthread_join(threads[i], nullptr);
        }
    }
};

#endif