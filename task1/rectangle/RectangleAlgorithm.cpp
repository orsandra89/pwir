#include "RectangleAlgorithm.h"

// RectangleAlgorithm::RectangleAlgorithm() : IntegralAlgorithm() {}

double RectangleAlgorithm::integration(Polynomial& poly, double a, double b, int n) const {
    std::cout << "Rectangle algorithm" << std::endl;
    double integral = 0.0;
    double interval_width = (b - a) / n;
    for (int i = 0; i < n; ++i) {
        double x_mid = a + (i + 0.5) * interval_width;
        integral += poly.evaluate(x_mid) * interval_width;
    }
    return integral;
}
