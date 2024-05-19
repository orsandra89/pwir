#include "Polynomial.h"

Polynomial::Polynomial() : coefficients({}) {}
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

void Polynomial::print() const {
    bool first = true;
    int n = coefficients.size();  // Length of the coefficient array

    for (int i = n - 1; i >= 0; --i) {
        if (coefficients[i] != 0) {  // Only print non-zero terms
            // Handle the sign, except for the first term printed
            if (!first) {
                std::cout << (coefficients[i] > 0 ? " + " : " - ");
            } else if (coefficients[i] < 0) {
                std::cout << "-";
            }

            // Handle the coefficient and variable
            int absCoeff = std::abs(coefficients[i]);
            if (i == 0) {
                // Constant term
                std::cout << absCoeff;
            } else if (i == 1) {
                // The linear term
                if (absCoeff != 1) std::cout << absCoeff;
                std::cout << "x";
            } else {
                // Terms with power greater than 1
                if (absCoeff != 1) std::cout << absCoeff;
                std::cout << "x^" << i;
            }

            first = false;
        }
    }

    if (first) {
        std::cout << "0";  // Print "0" if all coefficients are zero
    }

    std::cout << std::endl;
}