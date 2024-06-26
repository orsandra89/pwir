#include <vector>
#include <iostream>
#include "polynomial/QuadraticPolynomial.h"
#include "IntegralAlgorithm.h"
#include "simpson/test/SimpsonAlgorithmMPITest.h"

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    QuadraticPolynomial* poly = new QuadraticPolynomial(2, -3, 1);

    // Define the range [a, b] and the number of intervals
    double a = 0.0;
    double b = 1000.0;
    int num_intervals = 1000000;

    SimpsonAlgorithmMPITest ra = SimpsonAlgorithmMPITest(poly, a, b, "simpson_mpi_algorithm.csv");

    ra.execute(1000000, 10000000, 1000000);
    std::cout << "Finish calculation" << std::endl;

    MPI_Finalize();

    return 0;
}