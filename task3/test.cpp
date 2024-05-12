#include "Matrix.cpp"
#include <map>
#include <cmath>

void writeVectorToFile(const std::string& filePath, const std::vector<double> result) {
        std::ofstream outFile(filePath);
        
        if (!outFile.is_open()) {
            std::cerr << "Error: Unable to open file for writing: " << filePath << std::endl;
            return;
        }

        outFile << result.size() << "\n";

        for (int i = 0; i < result.size(); ++i) {
            outFile << std::fixed << std::setprecision(6) << result[i];
            if (i != result.size() - 1) {
                outFile << ";";
            }
        }

        outFile.close();
    }

int main(int argc, char *argv[]) {
    std::map<int, std::string> algorithmMap = {
        {0, "GaussSequential"},
        {1, "GaussOpenMP"},
        {2, "GaussThreadLibrary"}
    };
    if (argc < 4) {
        std::cout << "Available integration algorithms:" << std::endl;
        for (const auto& pair : algorithmMap) {
            std::cout << pair.first << ". " << pair.second << std::endl;
        }
        std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <algorithm> <result_filename> <thread_num>" << std::endl;
        return 1;
    }

    std::string fileName1 = argv[1];
    int algorithm = std::stoi(argv[2]);

    std::string resultFileName2 = argv[3];

    if (algorithm == 0){

        Matrix mat1(fileName1);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        auto start = std::chrono::steady_clock::now();
        
        std::vector<double> result = mat1.gauss_elimination();

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result vector:" << std::endl;
        
        for (double sol : result) {
            std::cout << std::fixed << std::setprecision(6) << sol << ",";
        }
        std::cout << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        writeVectorToFile(resultFileName2, result);
    } else if (algorithm == 1) {
        if (argc != 5) {
            std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <algorithm> <result_filename> <thread_num>" << std::endl;
            return 1;
        }

        int numThread = std::stoi(argv[4]);

        Matrix mat1(fileName1);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        auto start = std::chrono::steady_clock::now();
        
        std::vector<double> result = mat1.gauss_elimination_openmp(numThread);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result vector:" << std::endl;
        
        for (double sol : result) {
            std::cout << std::fixed << std::setprecision(6) << sol << ",";
        }
        std::cout << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        writeVectorToFile(resultFileName2, result);
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

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        auto start = std::chrono::steady_clock::now();
        
        std::vector<double> result = mat1.gauss_elimination();

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result vector:" << std::endl;
        
        for (double sol : result) {
            std::cout << std::fixed << std::setprecision(6) << sol << ",";
        }
        std::cout << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        writeVectorToFile(resultFileName2, result);
    } else {
        std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <algorithm> <result_filename> <thread_num>" << std::endl;
            return 1;
    }

    return 0;
}