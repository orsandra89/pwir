#include "TrapezoidAlgorithmTest.h"

TrapezoidAlgorithmTest::TrapezoidAlgorithmTest(Polynomial* poly, double a, double b, std::string filename) : IntegralTest(poly, a, b, filename) {
    this->poly = poly;
    this->a = a;
    this->b = b;
    this->filename = filename;
    this->algorithm = TrapezoidAlgorithm();
}

CsvRecord TrapezoidAlgorithmTest::test(int num_intervals) {
    auto start = std::chrono::steady_clock::now();
    
    algorithm.integration(*poly, a, b, num_intervals);

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "TrapezoidAlgorithm", elapsed_time, num_intervals };
}
