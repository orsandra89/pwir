#include "Matrix.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <matrix2_filename>" << std::endl;
        return 1;
    }

    std::string fileName1 = argv[1];
    std::string fileName2 = argv[2];

    Matrix mat1(fileName1);
    Matrix mat2(fileName2);

    std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
    mat1.print();

    std::cout << "Matrix loaded from " << fileName2 << ":" << std::endl;
    mat2.print();

    Matrix r = mat1 * mat2;

    std::cout << "Result matrix:" << std::endl;
    r.print();

    return 0;
}