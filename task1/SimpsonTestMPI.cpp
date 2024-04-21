#include <vector>
#include <iostream>
#include "QuadraticPolynomial.h"
#include "IntegralAlgorithm.h"
#include "SimpsonAlgorithmMPI.h"

int main() {
    // Example usage
    QuadraticPolynomial poly(2, -3, 1);

    // Define the range [a, b] and the number of intervals
    double a = 0.0;
    double b = 3.0;
    int num_intervals = 1000;

    SimpsonAlgorithmMPI ra = SimpsonAlgorithmMPI();

    // Calculate the integral using the rectangle method
    double result = ra.integration(poly, a, b, num_intervals);
    std::cout << "Integral of the polynomial over the range [" << a << ", " << b << "]: " << result << std::endl;

    return 0;
}