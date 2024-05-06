#ifndef THREADLIB_MATRIX_TEST_H
#define THREADLIB_MATRIX_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "Matrix.cpp"
#include "MatrixTest.h"

class TreadLibMatrixTest : public MatrixTest {
    private:
        std::string filename;

    public:
        TreadLibMatrixTest(std::string filename);
        CsvRecord test(Matrix m1, Matrix m2, int threadNum) override;
};

#endif