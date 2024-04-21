#ifndef TRAPEZOID_ALGORITHM_TEST_H
#define TRAPEZOID_ALGORITHM_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "../../polynomial/Polynomial.h"
#include "../../IntegralTest.h"
#include "../TrapezoidAlgorithm.h"

class TrapezoidAlgorithmTest : public IntegralTest {
    private:
        Polynomial* poly;
        TrapezoidAlgorithm algorithm;
        double a;
        double b;
        std::string filename;
    public:
        TrapezoidAlgorithmTest(Polynomial* poly, double a, double b, std::string filename);
        // Function to calculate time for one execution and return CsvRecord
        CsvRecord test(int num_intervals) override;
};

#endif