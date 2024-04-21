#ifndef SIMPSON_ALGORITHM_TEST_H
#define SIMPSON_ALGORITHM_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "../../polynomial/Polynomial.h"
#include "../../IntegralTest.h"
#include "../SimpsonAlgorithm.h"

class SimpsonAlgorithmTest : public IntegralTest {
    private:
        Polynomial* poly;
        SimpsonAlgorithm algorithm;
        double a;
        double b;
        std::string filename;
    public:
        SimpsonAlgorithmTest(Polynomial* poly, double a, double b, std::string filename);
        CsvRecord test(int num_intervals) override;
};

#endif