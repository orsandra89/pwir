#include "Matrix.cpp"
#include "SequentialMatrixTest.h"

int main(int argc, char *argv[]) {
    SequentialMatrixTest t("sequential_results.csv");

    t.execute(1000, 2, 10, 1);

    std::cout << "Test finished!!!" << std::endl;

    return 0;
}