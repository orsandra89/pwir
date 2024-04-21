#ifndef INTEGRAL_TEST_H
#define INTEGRAL_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "polynomial/Polynomial.h"
#include "IntegralAlgorithm.h"

struct CsvRecord {
    std::string name;
    double time;
    int num_intervals;
    double result;
};

class IntegralTest {
    private:
        Polynomial* poly;
        double a;
        double b;
        std::string filename;

    public:
        IntegralTest(Polynomial* poly, double a, double b, std::string filename);
        ~IntegralTest();
       
        virtual CsvRecord test(int num_intervals);

        std::vector<CsvRecord> performTests(int min_intervals, int max_intervals, int interval_step);

        void writeToCsv(const std::vector<CsvRecord>& data, const std::string& filename);

        void execute(int min_intervals, int max_intervals, int interval_step);
};

#endif