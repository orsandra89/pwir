#ifndef RECTANGLE_ALGORITHM_OPENMP_TEST_H
#define RECTANGLE_ALGORITHM_OPENMP_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "../../polynomial/Polynomial.h"
#include "../../IntegralTest.h"
#include "../RectangleAlgorithmOpenMP.h"

class RectangleAlgorithmOpenMPTest : public IntegralTest {
    private:
        Polynomial* poly;
        RectangleAlgorithmOpenMP algorithm;
        double a;
        double b;
        std::string filename;
    public:
        RectangleAlgorithmOpenMPTest(Polynomial* poly, double a, double b, std::string filename);
        CsvRecord test(int num_intervals) override;
};

#endif