#include "MPMatrixTest.h"

MPMatrixTest::MPMatrixTest(std::string filename) : MatrixTest(filename) {}

CsvRecord MPMatrixTest::test(Matrix m1, Matrix m2, int threadNum) {
    auto start = std::chrono::steady_clock::now();
    
    Matrix result = m1.matrixMultiply(m2, threadNum);

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "MPTest", elapsed_time, m1.getRowsCount(), m1.getColumnsCount(), threadNum };
}
