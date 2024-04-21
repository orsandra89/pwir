#include <vector>
#include <iostream>
#include "QuadraticPolynomial.h"
#include "IntegralAlgorithm.h"
#include "TrapezoidAlgorithmOpenMP.h"

int main() {
    // Example usage
    std::vector<int> coeffs = {1, -3, 2}; // Represents the polynomial x^2 - 3x + 2
    QuadraticPolynomial poly(2, -3, 1);

    // Define the range [a, b] and the number of intervals
    double a = 0.0;
    double b = 3.0;
    int num_intervals = 1000;

    TrapezoidAlgorithmOpenMP ra = TrapezoidAlgorithmOpenMP();

    // Calculate the integral using the rectangle method
    double result = ra.integration(poly, a, b, num_intervals);
    std::cout << "Integral of the polynomial over the range [" << a << ", " << b << "]: " << result << std::endl;

    return 0;
}