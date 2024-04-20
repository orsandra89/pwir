#include "Polynomial.h"

Polynomial::Polynomial(std::vector<double> coeffs) : coefficients(coeffs) {}

double Polynomial::evaluate(double x) const {
    double result = 0.0;
    double power_of_x = 1.0;
    for (double coeff : coefficients) {
        result += coeff * power_of_x;
        power_of_x *= x;
    }
    return result;
}