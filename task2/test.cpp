#include "Matrix.cpp"
#include <map>
#include <cmath>

int main(int argc, char *argv[]) {
    std::map<int, std::string> algorithmMap = {
        {0, "Sequential"},
        {1, "OpenMP"},
        {2, "ThreadLibrary"}
    };
    if (argc < 4) {
        std::cout << "Available integration algorithms:" << std::endl;
        for (const auto& pair : algorithmMap) {
            std::cout << pair.first << ". " << pair.second << std::endl;
        }
        std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <matrix2_filename> <algorithm> <result_filename> <thread_num>" << std::endl;
        return 1;
    }

    std::string fileName1 = argv[1];
    std::string fileName2 = argv[2];
    int algorithm = std::stoi(argv[3]);

    std::string resultFileName2 = argv[4];

    if (algorithm == 0){

        Matrix mat1(fileName1);
        Matrix mat2(fileName2);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        std::cout << "Matrix loaded from " << fileName2 << ":" << std::endl;
        mat2.print();

        auto start = std::chrono::steady_clock::now();
        
        Matrix result = mat1 * mat2;

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result matrix:" << std::endl;
        result.print();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        result.writeToFile(resultFileName2);
    } else if (algorithm == 1) {
        if (argc != 6) {
            std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <matrix2_filename> <algorithm> <result_filename> <thread_num>" << std::endl;
            return 1;
        }

        int numThread = std::stoi(argv[5]);

        Matrix mat1(fileName1);
        Matrix mat2(fileName2);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        std::cout << "Matrix loaded from " << fileName2 << ":" << std::endl;
        mat2.print();

        auto start = std::chrono::steady_clock::now();
        
        Matrix result = mat1.matrixMultiply(mat2, numThread);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result matrix:" << std::endl;
        result.print();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        result.writeToFile(resultFileName2);
    } else if (algorithm == 2) {
        if (argc != 6) {
            std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <matrix2_filename> <algorithm> <result_filename> <thread_num>" << std::endl;
            return 1;
        }

        int numThread = std::stoi(argv[5]);

        Matrix mat1(fileName1);
        Matrix mat2(fileName2);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        std::cout << "Matrix loaded from " << fileName2 << ":" << std::endl;
        mat2.print();

        auto start = std::chrono::steady_clock::now();
        
        Matrix result = mat1.matrixMultiplyThreadLibrary(mat2, numThread);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result matrix:" << std::endl;
        result.print();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        result.writeToFile(resultFileName2);
    } else {
        std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <matrix2_filename> <algorithm> <result_filename> <thread_num>" << std::endl;
            return 1;
    }

    return 0;
}