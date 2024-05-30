#ifndef LINE_H
#define LINE_H

#include <cmath>
#include <pthread.h>
#include "BPM.cpp"

class Line : public Drawable {
private:
    int x0, y0, x1, y1;
    uint8_t r, g, b;

public:
    Line(int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b)
        : x0(x0), y0(y0), x1(x1), y1(y1), r(r), g(g), b(b) {}

    void draw(BMP &image) const override {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int steps = std::max(std::abs(dx), std::abs(dy));
        
        double xIncrement = static_cast<double>(dx) / steps;
        double yIncrement = static_cast<double>(dy) / steps;

        for (int i = 0; i <= steps; ++i) {
            double x = x0 + i * xIncrement;
            double y = y0 + i * yIncrement;
            image.setPixel(static_cast<int>(round(x)), static_cast<int>(round(y)), r, g, b);
        }
    }

    void draw_openmp(BMP &image, int thread_num) const override {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int steps = std::max(std::abs(dx), std::abs(dy));
        
        double xIncrement = static_cast<double>(dx) / steps;
        double yIncrement = static_cast<double>(dy) / steps;

        #pragma omp parallel for num_threads(thread_num)
        for (int i = 0; i <= steps; ++i) {
            double x = x0 + i * xIncrement;
            double y = y0 + i * yIncrement;
            image.setPixel(static_cast<int>(round(x)), static_cast<int>(round(y)), r, g, b);
        }
    }

    void draw_threadlib(BMP &image, int thread_num) const override {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int steps = std::max(std::abs(dx), std::abs(dy));
        
        double xIncrement = static_cast<double>(dx) / steps;
        double yIncrement = static_cast<double>(dy) / steps;

        int segmentLength = steps / thread_num;
        int remainder = steps % thread_num;

        struct ThreadData {
            BMP* image;
            int x0, y0;
            double xIncrement, yIncrement;
            int startStep, endStep;
            uint8_t r, g, b;
        };

        auto threadFunc = [](void* arg) -> void* {
            ThreadData* data = static_cast<ThreadData*>(arg);
            for (int i = data->startStep; i <= data->endStep; ++i) {
                double x = data->x0 + i * data->xIncrement;
                double y = data->y0 + i * data->yIncrement;
                data->image->setPixel(static_cast<int>(round(x)), static_cast<int>(round(y)), data->r, data->g, data->b);
            }
            return nullptr;
        };

        std::vector<pthread_t> threads(thread_num);
        std::vector<ThreadData> threadData(thread_num);

        int startStep = 0;
        for (int i = 0; i < thread_num; ++i) {
            int endStep = startStep + segmentLength - 1;
            if (i < remainder) {
                endStep += 1;
            }
            threadData[i] = { &image, x0, y0, xIncrement, yIncrement, startStep, endStep, r, g, b };
            pthread_create(&threads[i], nullptr, threadFunc, &threadData[i]);
            startStep = endStep + 1;
        }

        for (int i = 0; i < thread_num; ++i) {
            pthread_join(threads[i], nullptr);
        }
    }
};

#endif