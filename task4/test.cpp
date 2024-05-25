#include "Matrix.cpp"
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

int main(int argc, char *argv[]) {
    std::map<int, std::string> algorithmMap = {
        {0, "SortSequential"},
        {1, "SortOpenMP"},
        {2, "SortThreadLibrary"},
        {3, "CompleteSortSequential"},
        {4, "CompleteSortOpenMP"},
        {5, "CompleteSortThreadLibrary"}
    };

    if (argc < 2) {
        std::cout << "Available sort algorithms:" << std::endl;
        for (const auto& pair : algorithmMap) {
            std::cout << pair.first << ". " << pair.second << std::endl;
        }
        std::cerr << "Usage: " << argv[0] << " <algorithm>" << std::endl;
        return 1;
    }
    int algorithm = std::stoi(argv[1]);

    if (algorithm == 0){
        Matrix mat1(0,0,0.0);
        std::string resultPath;
        if (argc == 7)
        {
            int rows = std::stoi(argv[2]);
            int cols = std::stoi(argv[3]);
            double min = std::stod(argv[4]);
            double max = std::stod(argv[5]);
            resultPath = argv[6];

            mat1 = Matrix(rows, cols, min, max);
        } else if (argc == 4) {
            std::string matrixPath = argv[2];
            resultPath = argv[3];

            std::cout << "Matrix loaded from " << matrixPath << ":" << std::endl;
            mat1 = Matrix(matrixPath);
            mat1.print();
        } else {
            std::cout << "Available sort algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <algorithm> <m> <n> <min> <max> <result_matrix_path>" << std::endl;
            std::cerr << "or" << std::endl;
            std::cerr << "Usage: " << argv[0] << " <algorithm> <input_matrix_path> <result_matrix_path>" << std::endl;
            return 1;
        }

        auto start = std::chrono::steady_clock::now();
        
        Matrix result = mat1.sort();

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        result.writeToFile(resultPath);
    } else if (algorithm == 1) {
        Matrix mat1(0,0,0.0);
        std::string resultPath;
        int thread_num;
        if (argc == 8)
        {
            int rows = std::stoi(argv[2]);
            int cols = std::stoi(argv[3]);
            double min = std::stod(argv[4]);
            double max = std::stod(argv[5]);
            resultPath = argv[6];
            thread_num = std::stoi(argv[7]);

            mat1 = Matrix(rows, cols, min, max);
        } else if (argc == 5) {
            std::string matrixPath = argv[2];
            resultPath = argv[3];
            thread_num = std::stoi(argv[4]);

            std::cout << "Matrix loaded from " << matrixPath << ":" << std::endl;
            mat1 = Matrix(matrixPath);
            mat1.print();
        } else {
            std::cout << "Available sort algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <algorithm> <m> <n> <min> <max> <result_matrix_path> <thread_num>" << std::endl;
            std::cerr << "or" << std::endl;
            std::cerr << "Usage: " << argv[0] << " <algorithm> <input_matrix_path> <result_matrix_path> <thread_num>" << std::endl;
            return 1;
        }

        auto start = std::chrono::steady_clock::now();
        
        Matrix result = mat1.sort_openm(thread_num);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        result.writeToFile(resultPath);
    } else if (algorithm == 2) {
        Matrix mat1(0,0,0.0);
        std::string resultPath;
        int thread_num;
        if (argc == 8)
        {
            int rows = std::stoi(argv[2]);
            int cols = std::stoi(argv[3]);
            double min = std::stod(argv[4]);
            double max = std::stod(argv[5]);
            resultPath = argv[6];
            thread_num = std::stoi(argv[7]);

            mat1 = Matrix(rows, cols, min, max);
        } else if (argc == 5) {
            std::string matrixPath = argv[2];
            resultPath = argv[3];
            thread_num = std::stoi(argv[4]);

            std::cout << "Matrix loaded from " << matrixPath << ":" << std::endl;
            mat1 = Matrix(matrixPath);
            mat1.print();
        } else {
            std::cout << "Available sort algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <algorithm> <m> <n> <min> <max> <result_matrix_path> <thread_num>" << std::endl;
            std::cerr << "or" << std::endl;
            std::cerr << "Usage: " << argv[0] << " <algorithm> <input_matrix_path> <result_matrix_path> <thread_num>" << std::endl;
            return 1;
        }

        auto start = std::chrono::steady_clock::now();
        
        Matrix result = mat1.sort_threadlib(thread_num);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        result.writeToFile(resultPath);
    } else if (algorithm == 3){
        Matrix mat1(0,0,0.0);
        std::string resultPath;
        if (argc == 7)
        {
            int rows = std::stoi(argv[2]);
            int cols = std::stoi(argv[3]);
            double min = std::stod(argv[4]);
            double max = std::stod(argv[5]);
            resultPath = argv[6];

            mat1 = Matrix(rows, cols, min, max);
        } else if (argc == 4) {
            std::string matrixPath = argv[2];
            resultPath = argv[3];

            std::cout << "Matrix loaded from " << matrixPath << ":" << std::endl;
            mat1 = Matrix(matrixPath);
            mat1.print();
        } else {
            std::cout << "Available sort algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <algorithm> <m> <n> <min> <max> <result_matrix_path>" << std::endl;
            std::cerr << "or" << std::endl;
            std::cerr << "Usage: " << argv[0] << " <algorithm> <input_matrix_path> <result_matrix_path>" << std::endl;
            return 1;
        }

        auto start = std::chrono::steady_clock::now();
        
        Matrix result = mat1.complete_sort();

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        result.writeToFile(resultPath);
    } else if (algorithm == 4) {
        Matrix mat1(0,0,0.0);
        std::string resultPath;
        int thread_num;
        if (argc == 8)
        {
            int rows = std::stoi(argv[2]);
            int cols = std::stoi(argv[3]);
            double min = std::stod(argv[4]);
            double max = std::stod(argv[5]);
            resultPath = argv[6];
            thread_num = std::stoi(argv[7]);

            mat1 = Matrix(rows, cols, min, max);
        } else if (argc == 5) {
            std::string matrixPath = argv[2];
            resultPath = argv[3];
            thread_num = std::stoi(argv[4]);

            std::cout << "Matrix loaded from " << matrixPath << ":" << std::endl;
            mat1 = Matrix(matrixPath);
            mat1.print();
        } else {
            std::cout << "Available sort algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <algorithm> <m> <n> <min> <max> <result_matrix_path> <thread_num>" << std::endl;
            std::cerr << "or" << std::endl;
            std::cerr << "Usage: " << argv[0] << " <algorithm> <input_matrix_path> <result_matrix_path> <thread_num>" << std::endl;
            return 1;
        }

        auto start = std::chrono::steady_clock::now();
        
        Matrix result = mat1.complete_sort_openm(thread_num);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        result.writeToFile(resultPath);
    } else if (algorithm == 5) {
        Matrix mat1(0,0,0.0);
        std::string resultPath;
        int thread_num;
        if (argc == 8)
        {
            int rows = std::stoi(argv[2]);
            int cols = std::stoi(argv[3]);
            double min = std::stod(argv[4]);
            double max = std::stod(argv[5]);
            resultPath = argv[6];
            thread_num = std::stoi(argv[7]);

            mat1 = Matrix(rows, cols, min, max);
        } else if (argc == 5) {
            std::string matrixPath = argv[2];
            resultPath = argv[3];
            thread_num = std::stoi(argv[4]);

            std::cout << "Matrix loaded from " << matrixPath << ":" << std::endl;
            mat1 = Matrix(matrixPath);
            mat1.print();
        } else {
            std::cout << "Available sort algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <algorithm> <m> <n> <min> <max> <result_matrix_path> <thread_num>" << std::endl;
            std::cerr << "or" << std::endl;
            std::cerr << "Usage: " << argv[0] << " <algorithm> <input_matrix_path> <result_matrix_path> <thread_num>" << std::endl;
            return 1;
        }

        auto start = std::chrono::steady_clock::now();
        
        Matrix result = mat1.complete_sort_threadlib(thread_num);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        result.writeToFile(resultPath);
    } else {
        std::cout << "Available sort algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <algorithm> <m> <n> <min> <max> <result_matrix_path>" << std::endl;
            std::cerr << "or" << std::endl;
            std::cerr << "Usage: " << argv[0] << " <algorithm> <input_matrix_path> <result_matrix_path>" << std::endl;
            return 1;
    }

    return 0;
}