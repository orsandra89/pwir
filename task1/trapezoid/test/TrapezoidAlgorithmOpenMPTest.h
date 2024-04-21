#ifndef TRAPEZOID_ALGORITHM_OPENMP_TEST_H
#define TRAPEZOID_ALGORITHM_OPENMP_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "../../polynomial/Polynomial.h"
#include "../../IntegralTest.h"
#include "../TrapezoidAlgorithmOpenMP.h"

class TrapezoidAlgorithmOpenMPTest : public IntegralTest {
    private:
        Polynomial* poly;
        TrapezoidAlgorithmOpenMP algorithm;
        double a;
        double b;
        std::string filename;
    public:
        TrapezoidAlgorithmOpenMPTest(Polynomial* poly, double a, double b, std::string filename);
        CsvRecord test(int num_intervals) override;
};

#endif