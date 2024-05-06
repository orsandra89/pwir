#include "Matrix.cpp"
#include "TreadLibMatrixTest.h"

int main(int argc, char *argv[]) {
    TreadLibMatrixTest t("threadlib_results.csv");

    t.execute(1000, 2, 10, 1);

    std::cout << "Test finished!!!" << std::endl;

    return 0;
}