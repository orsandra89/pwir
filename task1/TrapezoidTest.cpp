#include <vector>
#include <iostream>
#include "polynomial/QuadraticPolynomial.h"
#include "IntegralAlgorithm.h"
#include "trapezoid/test/TrapezoidAlgorithmTest.h"

int main() {
    QuadraticPolynomial* poly = new QuadraticPolynomial(2, -3, 1);

    // Define the range [a, b] and the number of intervals
    double a = 0.0;
    double b = 1000.0;
    int num_intervals = 1000000;

    TrapezoidAlgorithmTest ra = TrapezoidAlgorithmTest(poly, a, b, "trapezoid_algorithm.csv");

    ra.execute(1000000, 10000000, 1000000);
    std::cout << "Finish calculation" << std::endl;

    return 0;
}