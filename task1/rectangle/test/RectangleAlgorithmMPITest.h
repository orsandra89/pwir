#ifndef RECTANGLE_ALGORITHM_MPI_TEST_H
#define RECTANGLE_ALGORITHM_MPI_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "../../polynomial/Polynomial.h"
#include "../../IntegralTest.h"
#include "../RectangleAlgorithmMPI.h"

class RectangleAlgorithmMPITest : public IntegralTest {
    private:
        Polynomial* poly;
        RectangleAlgorithmMPI algorithm;
        double a;
        double b;
        std::string filename;
    public:
        RectangleAlgorithmMPITest(Polynomial* poly, double a, double b, std::string filename);
        // Function to calculate time for one execution and return CsvRecord
        CsvRecord test(int num_intervals) override;
};

#endif