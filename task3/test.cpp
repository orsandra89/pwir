#include "Matrix.cpp"
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

struct LogRecord {
    std::string algorithm;
    std::string execution_date;
    double execution_time;
    int N;
    bool full_pivoting;
    int num_threads;
};

void writeLogRecordToFile(const LogRecord& record, const std::string& filename) {
    std::ofstream outfile(filename, std::ios::app); // Open file in append mode
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return;
    }

    outfile << "#### Log Record ####" << std::endl;
    outfile << "algorithm: " << record.algorithm << std::endl;
    outfile << "execution date: " << record.execution_date << std::endl;
    outfile << "execution time: " << record.execution_time << std::endl;
    outfile << "number of equations: " << record.N << std::endl;
    outfile << "full pivoting: " << (record.full_pivoting ? "true" : "false") << std::endl;
    outfile << "number of threads: " << record.num_threads << std::endl;
    outfile << "#### Log Record End ####" << std::endl;
    outfile.close();
}

bool stringToBool(const std::string& str) {
    static const std::map<std::string, bool> trueStringMap{
        {"true", true}, {"1", true}, {"yes", true}, {"y", true},
    };

    static const std::map<std::string, bool> falseStringMap{
        {"false", false}, {"0", false}, {"no", false}, {"n", false},
    };

    // Convert the input string to lowercase for case-insensitive comparison
    std::string lowerStr;
    for (char ch : str) {
        lowerStr += std::tolower(ch);
    }

    // Check if the string exists in the true or false mapping
    auto trueIt = trueStringMap.find(lowerStr);
    if (trueIt != trueStringMap.end()) {
        return trueIt->second;
    }

    auto falseIt = falseStringMap.find(lowerStr);
    if (falseIt != falseStringMap.end()) {
        return falseIt->second;
    }

    // If the string does not match any recognized boolean representation, return false by default
    return false;
}

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
        {2, "GaussThreadLibrary"},
        {3, "GaussJourdanSequential"},
        {4, "GaussJourdanOpenMP"},
        {5, "GaussJourdanThreadLibrary"}
    };

    time_t now = time(0);
    tm* local_time = localtime(&now);
    char date_str[20];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", local_time);

    LogRecord record;
    bool log;

    if (argc < 5) {
        std::cout << "Available integration algorithms:" << std::endl;
        for (const auto& pair : algorithmMap) {
            std::cout << pair.first << ". " << pair.second << std::endl;
        }
        std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <algorithm> <result_filename> <complete_pivoting>" << std::endl;
        return 1;
    }

    std::string fileName1 = argv[1];
    std::string logfile;
    int algorithm = std::stoi(argv[2]);

    std::string resultFileName2 = argv[3];

    bool pivoting = stringToBool(argv[4]);

    if (algorithm == 0){
        if (argc == 6)
        {
            log = true;
            logfile = argv[5];
        } else {
            log = false;
        }

        Matrix mat1(fileName1);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        auto start = std::chrono::steady_clock::now();
        
        std::vector<double> result = mat1.gauss_elimination(pivoting);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result vector:" << std::endl;
        
        for (double sol : result) {
            std::cout << std::fixed << std::setprecision(6) << sol << ",";
        }
        std::cout << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        writeVectorToFile(resultFileName2, result);

        if (log) {
            record.algorithm = "Gauss Elimination";
            record.execution_date = date_str; // Example date, should be formatted according to your requirements
            record.execution_time = elapsed_time; // Example execution time
            record.N = mat1.getRowsCount(); // Example number of equations
            record.full_pivoting = true; // Example full pivoting flag
            record.num_threads = 1; // Example number of threads

            writeLogRecordToFile(record, logfile);
        }
    } else if (algorithm == 1) {
        if (argc == 7) {
            log = true;
            logfile = argv[6];
        } else if (argc == 6) {
            log = false;
        } else {
            std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <algorithm> <result_filename> <complete_pivoting> <thread_num>" << std::endl;
            return 1;
        }

        int numThread = std::stoi(argv[5]);

        Matrix mat1(fileName1);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        auto start = std::chrono::steady_clock::now();
        
        std::vector<double> result = mat1.gauss_elimination_openmp(numThread, pivoting);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result vector:" << std::endl;
        
        for (double sol : result) {
            std::cout << std::fixed << std::setprecision(6) << sol << ",";
        }
        std::cout << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        writeVectorToFile(resultFileName2, result);

        if (log) {
            record.algorithm = "Gauss Elimination with OpenMP";
            record.execution_date = date_str; // Example date, should be formatted according to your requirements
            record.execution_time = elapsed_time; // Example execution time
            record.N = mat1.getRowsCount(); // Example number of equations
            record.full_pivoting = true; // Example full pivoting flag
            record.num_threads = numThread; // Example number of threads

            writeLogRecordToFile(record, logfile);
        }
    } else if (algorithm == 2) {
        if (argc == 7) {
            log = true;
            logfile = argv[6];
        } else if (argc == 6) {
            log = false;
        } else {
            std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <algorithm> <result_filename> <complete_pivoting> <thread_num>" << std::endl;
            return 1;
        }

        int numThread = std::stoi(argv[5]);

        Matrix mat1(fileName1);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        auto start = std::chrono::steady_clock::now();
        
        std::vector<double> result = mat1.gauss_elimination_threadlib(numThread, pivoting);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result vector:" << std::endl;
        
        for (double sol : result) {
            std::cout << std::fixed << std::setprecision(6) << sol << ",";
        }
        std::cout << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        writeVectorToFile(resultFileName2, result);

        if (log) {
            record.algorithm = "Gauss Elimination with ThreadLib";
            record.execution_date = date_str; // Example date, should be formatted according to your requirements
            record.execution_time = elapsed_time; // Example execution time
            record.N = mat1.getRowsCount(); // Example number of equations
            record.full_pivoting = true; // Example full pivoting flag
            record.num_threads = numThread; // Example number of threads

            writeLogRecordToFile(record, logfile);
        }
    } else if (algorithm == 3){
        if (argc == 6)
        {
            log = true;
            logfile = argv[5];
        } else {
            log = false;
        }

        Matrix mat1(fileName1);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        auto start = std::chrono::steady_clock::now();
        
        std::vector<double> result = mat1.gauss_jourdan_elimination(pivoting);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result vector:" << std::endl;
        
        for (double sol : result) {
            std::cout << std::fixed << std::setprecision(6) << sol << ",";
        }
        std::cout << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        writeVectorToFile(resultFileName2, result);

        if (log) {
            record.algorithm = "Gauss-Jourdan Elimination";
            record.execution_date = date_str; // Example date, should be formatted according to your requirements
            record.execution_time = elapsed_time; // Example execution time
            record.N = mat1.getRowsCount(); // Example number of equations
            record.full_pivoting = true; // Example full pivoting flag
            record.num_threads = 1; // Example number of threads

            writeLogRecordToFile(record, logfile);
        }
    } else if (algorithm == 4) {
        if (argc == 7) {
            log = true;
            logfile = argv[6];
        } else if (argc == 6) {
            log = false;
        } else {
            std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <algorithm> <result_filename> <complete_pivoting> <thread_num>" << std::endl;
            return 1;
        }

        int numThread = std::stoi(argv[5]);

        Matrix mat1(fileName1);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        auto start = std::chrono::steady_clock::now();
        
        std::vector<double> result = mat1.gauss_jourdan_elimination_openmp(numThread, pivoting);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result vector:" << std::endl;
        
        for (double sol : result) {
            std::cout << std::fixed << std::setprecision(6) << sol << ",";
        }
        std::cout << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        writeVectorToFile(resultFileName2, result);

        if (log) {
            record.algorithm = "Gauss-Jourdan Elimination with OpenMP";
            record.execution_date = date_str; // Example date, should be formatted according to your requirements
            record.execution_time = elapsed_time; // Example execution time
            record.N = mat1.getRowsCount(); // Example number of equations
            record.full_pivoting = true; // Example full pivoting flag
            record.num_threads = numThread; // Example number of threads

            writeLogRecordToFile(record, logfile);
        }
    } else if (algorithm == 5) {
        if (argc == 7) {
            log = true;
            logfile = argv[6];
        } else if (argc == 6) {
            log = false;
        } else {
            std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <matrix1_filename> <algorithm> <result_filename> <complete_pivoting> <thread_num>" << std::endl;
            return 1;
        }

        int numThread = std::stoi(argv[5]);

        Matrix mat1(fileName1);

        std::cout << "Matrix loaded from " << fileName1 << ":" << std::endl;
        mat1.print();

        auto start = std::chrono::steady_clock::now();
        
        std::vector<double> result = mat1.gauss_jordan_elimination_threadlib(numThread);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result vector:" << std::endl;
        
        for (double sol : result) {
            std::cout << std::fixed << std::setprecision(6) << sol << ",";
        }
        std::cout << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        writeVectorToFile(resultFileName2, result);

        if (log) {
            record.algorithm = "Gauss Elimination with ThreadLib";
            record.execution_date = date_str; // Example date, should be formatted according to your requirements
            record.execution_time = elapsed_time; // Example execution time
            record.N = mat1.getRowsCount(); // Example number of equations
            record.full_pivoting = true; // Example full pivoting flag
            record.num_threads = numThread; // Example number of threads

            writeLogRecordToFile(record, logfile);
        }
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