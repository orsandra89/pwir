#include "IntegralTest.h"

IntegralTest::IntegralTest(Polynomial* poly, double a, double b, std::string filename) : poly(poly), a(a), b(b), filename(filename) {}

IntegralTest::~IntegralTest() {
    std::cout << "Delete IntegralTest" << std::endl;
    delete poly;
}

CsvRecord IntegralTest::test(int num_intervals) {
    return {"", 0.0, 0, 0.0};
}

std::vector<CsvRecord> IntegralTest::performTests(int min_intervals, int max_intervals, int interval_step) {
    std::vector<CsvRecord> records;
    for (int i = min_intervals; i <= max_intervals; i += interval_step) {
        records.push_back(test(i));
    }
    return records;
}

void IntegralTest::writeToCsv(const std::vector<CsvRecord>& data, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file << "Name,Time,Num_Intervals,Result\n";
    for (const auto& record : data) {
        file << record.name << "," << record.time << "," << record.num_intervals << "," << record.result << "\n";
    }
    file.close();
    std::cout << "Data written to " << filename << std::endl;
}

void IntegralTest::execute(int min_intervals, int max_intervals, int interval_step) {
    auto records = performTests(min_intervals, max_intervals, interval_step);
    writeToCsv(records, filename);
}
