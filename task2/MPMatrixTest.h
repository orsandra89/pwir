#ifndef MP_MATRIX_TEST_H
#define MP_MATRIX_TEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "Matrix.cpp"
#include "MatrixTest.h"

class MPMatrixTest : public MatrixTest {
    private:
        std::string filename;

    public:
        MPMatrixTest(std::string filename);
        CsvRecord test(Matrix m1, Matrix m2) override;
};

#endif