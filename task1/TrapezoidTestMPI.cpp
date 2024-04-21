#include <vector>
#include <iostream>
#include "polynomial/QuadraticPolynomial.h"
#include "IntegralAlgorithm.h"
#include "trapezoid/test/TrapezoidAlgorithmMPITest.h"

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    QuadraticPolynomial* poly = new QuadraticPolynomial(2, -3, 1);

    // Define the range [a, b] and the number of intervals
    double a = 0.0;
    double b = 1000.0;
    int num_intervals = 1000000;

    TrapezoidAlgorithmMPITest ra = TrapezoidAlgorithmMPITest(poly, a, b, "trapezoid_mpi_algorithm.csv");

    ra.execute(1000000, 10000000, 1000000);
    std::cout << "Finish calculation" << std::endl;

    MPI_Finalize();

    return 0;
}