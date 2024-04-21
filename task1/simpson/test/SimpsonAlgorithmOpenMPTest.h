#ifndef SIMPSON_ALGORITHM_OPENMP_TEST_H
#define SIMPSON_ALGORITHM_OPENMP_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "../../polynomial/Polynomial.h"
#include "../../IntegralTest.h"
#include "../SimpsonAlgorithmOpenMP.h"

class SimpsonAlgorithmOpenMPTest : public IntegralTest {
    private:
        Polynomial* poly;
        SimpsonAlgorithmOpenMP algorithm;
        double a;
        double b;
        std::string filename;
    public:
        SimpsonAlgorithmOpenMPTest(Polynomial* poly, double a, double b, std::string filename);
        CsvRecord test(int num_intervals) override;
};

#endif