#include "Matrix.cpp"
#include "ThreadLibMatrixTest.h"

int main(int argc, char *argv[]) {
    ThreadLibMatrixTest t("threadlib_results.csv");

    t.execute(1000, 100, 100, 1);

    std::cout << "Test finished!!!" << std::endl;

    return 0;
}