#include <vector>
#include <iostream>
#include "QuadraticPolynomial.h"
#include "IntegralAlgorithm.h"
#include "SimpsonAlgorithmTest.h"

int main() {
    // Example usage
    std::vector<int> coeffs = {1, -3, 2}; // Represents the polynomial x^2 - 3x + 2
    QuadraticPolynomial* poly = new QuadraticPolynomial(2, -3, 1);

    // Define the range [a, b] and the number of intervals
    double a = 0.0;
    double b = 1000.0;
    int num_intervals = 1000000;

    SimpsonAlgorithmTest ra = SimpsonAlgorithmTest(poly, a, b, "simpson_algorithm.csv");

    // Calculate the integral using the rectangle method
    ra.execute(1000000, 10000000, 1000000);
    std::cout << "Finish calculation" << std::endl;

    // delete poly;

    return 0;
}