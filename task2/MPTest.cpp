#include "Matrix.h"
#include "MPMatrixTest.h"

int main(int argc, char *argv[]) {
    MPMatrixTest t("mp_results.csv");

    t.execute(500, 1500, 100);

    std::cout << "Test finished!!!" << std::endl;

    return 0;
}