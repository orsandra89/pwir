#ifndef MATRIX_TEST_H
#define MATRIX_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "Matrix.cpp"

struct CsvRecord {
    std::string name;
    double time;
    int num_rows;
    int num_columns;
    int thread_num;
};

class MatrixTest {
    private:
        std::string filename;

    public:
        MatrixTest(std::string filename);
        ~MatrixTest();
       
        virtual CsvRecord test(Matrix m1, Matrix m2);

        std::vector<CsvRecord> performTests(int min_intervals, int max_intervals, int interval_step);

        void writeToCsv(const std::vector<CsvRecord>& data, const std::string& filename);

        void execute(int min_intervals, int max_intervals, int interval_step);
};

#endif