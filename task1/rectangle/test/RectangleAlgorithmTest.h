#ifndef RECTANGLE_ALGORITHM_TEST_H
#define RECTANGLE_ALGORITHM_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "../../polynomial/Polynomial.h"
#include "../../IntegralTest.h"
#include "../RectangleAlgorithm.h"

class RectangleAlgorithmTest : public IntegralTest {
    private:
        Polynomial* poly;
        RectangleAlgorithm algorithm;
        double a;
        double b;
        std::string filename;
    public:
        RectangleAlgorithmTest(Polynomial* poly, double a, double b, std::string filename);
        CsvRecord test(int num_intervals) override;
};

#endif