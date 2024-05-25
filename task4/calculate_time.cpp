#include "Matrix.cpp"
#include <map>
#include <cmath>

struct CsvRecord {
    std::string name;
    double time;
    int rows;
    int cols;
    int thread_num;
};

CsvRecord sequential_test(Matrix m) {
    auto start = std::chrono::steady_clock::now();
    
    m.sort();

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "SequentialSort", elapsed_time, m.getRowsCount(), m.getColumnsCount(), 1 };
}

CsvRecord openmp_test(Matrix m, int num_threads) {
    auto start = std::chrono::steady_clock::now();
    
    m.sort_openm(num_threads);

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "OpenMPSort", elapsed_time, m.getRowsCount(), m.getColumnsCount(), num_threads };
}

CsvRecord threadlib_test(Matrix m, int num_threads) {
    auto start = std::chrono::steady_clock::now();
    
    m.sort_threadlib(num_threads);

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "ThreadLibSort", elapsed_time, m.getRowsCount(), m.getColumnsCount(), num_threads };
}

CsvRecord complete_sequential_test(Matrix m) {
    auto start = std::chrono::steady_clock::now();
    
    m.complete_sort();

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "SequentialCompleteSort", elapsed_time, m.getRowsCount(), m.getColumnsCount(), 1 };
}

CsvRecord complete_threadlib_test(Matrix m, int num_threads) {
    auto start = std::chrono::steady_clock::now();
    
    m.complete_sort_threadlib(num_threads);

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "ThreadLibCompleteSort", elapsed_time, m.getRowsCount(), m.getColumnsCount(), num_threads };
}

CsvRecord complete_openmp_test(Matrix m, int num_threads) {
    auto start = std::chrono::steady_clock::now();
    
    m.complete_sort_openm(num_threads);

    auto end = std::chrono::steady_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    return { "OpenMPCompleteSort", elapsed_time, m.getRowsCount(), m.getColumnsCount(), num_threads };
}

std::vector<CsvRecord> performTests(Matrix m, int min_size, int max_size, int interval_step) {
    std::vector<CsvRecord> records;
    records.push_back(sequential_test(m));
    for (int i = min_size; i <= max_size; i += interval_step) {
        records.push_back(openmp_test(m, i));
        records.push_back(threadlib_test(m, i));
    }
    records.push_back(complete_sequential_test(m));
    for (int i = min_size; i <= max_size; i += interval_step) {
        records.push_back(complete_openmp_test(m, i));
        records.push_back(complete_threadlib_test(m, i));
    }
    return records;
}

void writeToCsv(const std::vector<CsvRecord>& data, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file << "Name,Time,rows,cols,ThreadNum\n";
    for (const auto& record : data) {
        file << record.name << "," << record.time << "," << record.rows << "," << record.cols << "," << record.thread_num <<"\n";
    }
    file.close();
    std::cout << "Data written to " << filename << std::endl;
}

void execute(const std::string& filePath, Matrix m, int min_intervals, int max_intervals, int interval_step) {
    auto records = performTests(m, min_intervals, max_intervals, interval_step);
    writeToCsv(records, filePath);
}

int main(int argc, char *argv[]) {
    std::string filename = "n_1000_matrix.csv";
    std::string result_filename = "time_results.csv";
    Matrix m(filename);

    execute(result_filename, m, 2, 20, 1);

    std::cout << "Test finished!!!" << std::endl;

    return 0;
}