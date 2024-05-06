#include "MatrixTest.h"

MatrixTest::MatrixTest(std::string filename) : filename(filename) {}

MatrixTest::~MatrixTest() {
    std::cout << "Delete MatrixTest" << std::endl;
}

CsvRecord MatrixTest::test(Matrix m1, Matrix m2, int threadNum) {
    return {"", 0.0, 0, 0, 1};
}

std::vector<CsvRecord> MatrixTest::performTests(int size, int min_size, int max_size, int interval_step) {
    std::vector<CsvRecord> records;
    for (int i = min_size; i <= max_size; i += interval_step) {
        Matrix m1(size, size);
        Matrix m2(size, size);
        records.push_back(test(m1, m2, i));
    }
    return records;
}

void MatrixTest::writeToCsv(const std::vector<CsvRecord>& data, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file << "Name,Time,RowCount,ColumnCount,ThreadNum\n";
    for (const auto& record : data) {
        file << record.name << "," << record.time << "," << record.num_rows << "," << record.num_columns << "," << record.thread_num <<"\n";
    }
    file.close();
    std::cout << "Data written to " << filename << std::endl;
}

void MatrixTest::execute(int size, int min_intervals, int max_intervals, int interval_step) {
    auto records = performTests(size, min_intervals, max_intervals, interval_step);
    writeToCsv(records, filename);
}
