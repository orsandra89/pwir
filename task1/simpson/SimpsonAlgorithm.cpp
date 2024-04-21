#include "SimpsonAlgorithm.h"

double SimpsonAlgorithm::integration(Polynomial& poly, double a, double b, int n) const {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i <= n; ++i) {
        double x = a + i * h;
        double coeff = (i == 0 || i == n) ? 1 : (i % 2 == 0 ? 2 : 4);
        sum += coeff * poly.evaluate(x);
    }
    return sum * h / 3.0;
}
