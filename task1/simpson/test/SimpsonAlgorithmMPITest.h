#ifndef SIMPSON_ALGORITHM_MPI_TEST_H
#define SIMPSON_ALGORITHM_MPI_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "../../polynomial/Polynomial.h"
#include "../../IntegralTest.h"
#include "../SimpsonAlgorithmMPI.h"

class SimpsonAlgorithmMPITest : public IntegralTest {
    private:
        Polynomial* poly;
        SimpsonAlgorithmMPI algorithm;
        double a;
        double b;
        std::string filename;
    public:
        SimpsonAlgorithmMPITest(Polynomial* poly, double a, double b, std::string filename);
        // Function to calculate time for one execution and return CsvRecord
        CsvRecord test(int num_intervals) override;
};

#endif