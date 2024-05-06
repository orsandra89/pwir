#include "ThreadLibMatrixTest.h"

ThreadLibMatrixTest::ThreadLibMatrixTest(std::string filename) : MatrixTest(filename) {}

CsvRecord ThreadLibMatrixTest::test(Matrix m1, Matrix m2, int threadNum) {
    auto start = std::chrono::steady_clock::now();
    
    Matrix result = m1.matrixMultiplyThreadLibrary(m2, threadNum);

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "ThreadTest", elapsed_time, m1.getRowsCount(), m1.getColumnsCount(), threadNum };
}
