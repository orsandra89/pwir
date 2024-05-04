#include "SequentialMatrixTest.h"

SequentialMatrixTest::SequentialMatrixTest(std::string filename) : MatrixTest(filename) {}

CsvRecord SequentialMatrixTest::test(Matrix m1, Matrix m2) {
    auto start = std::chrono::steady_clock::now();
    
    Matrix result = m1 * m2;

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "SequentialTest", elapsed_time, m1.getRowsCount(), m1.getColumnsCount(), 1 };
}
