#ifndef SEQUENTIAL_MATRIX_TEST_H
#define SEQUENTIAL_MATRIX_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "Matrix.cpp"
#include "MatrixTest.h"

class SequentialMatrixTest : public MatrixTest {
    private:
        std::string filename;

    public:
        SequentialMatrixTest(std::string filename);
        CsvRecord test(Matrix m1, Matrix m2, int threadNum) override;
};

#endif