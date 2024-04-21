#include <vector>
#include <iostream>
#include "polynomial/QuadraticPolynomial.h"
#include "IntegralAlgorithm.h"
#include "simpson/test/SimpsonAlgorithmTest.h"

int main() {
    QuadraticPolynomial* poly = new QuadraticPolynomial(2, -3, 1);

    // Define the range [a, b] and the number of intervals
    double a = 0.0;
    double b = 1000.0;
    int num_intervals = 1000000;

    SimpsonAlgorithmTest ra = SimpsonAlgorithmTest(poly, a, b, "simpson_algorithm.csv");

    ra.execute(1000000, 10000000, 1000000);
    std::cout << "Finish calculation" << std::endl;

    return 0;
}