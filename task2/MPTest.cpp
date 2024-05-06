#include "Matrix.cpp"
#include "MPMatrixTest.h"

int main(int argc, char *argv[]) {
    MPMatrixTest t("mp_results.csv");

    t.execute(1000, 2, 10, 1);

    std::cout << "Test finished!!!" << std::endl;

    return 0;
}