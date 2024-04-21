#include "RectangleAlgorithmOpenMP.h"

double RectangleAlgorithmOpenMP::integration(Polynomial& poly, double a, double b, int n) const {
    double sum = 0.0;
    double h = (b - a) / n;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        double x1 = a + i * h;
        double x2 = a + (i + 1) * h;
        double mid = (x1 + x2) / 2;
        sum += poly.evaluate(mid) * h;
    }
    return sum;
}
