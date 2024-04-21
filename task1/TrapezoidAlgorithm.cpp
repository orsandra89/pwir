#include "TrapezoidAlgorithm.h"

double TrapezoidAlgorithm::integration(Polynomial& poly, double a, double b, int n) const {
    double h = (b - a) / static_cast<double>(n);
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        double x1 = a + i * h;
        double x2 = a + (i + 1) * h;
        sum += (poly.evaluate(x1) + poly.evaluate(x2)) * h / 2.0;
    }
    return sum;
}
