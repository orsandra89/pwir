#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <pthread.h>
#include <queue>
#include <random>

struct ThreadData {
    std::vector<std::vector<double>>* matrix;
    int startRow;
    int endRow;
};

struct Element {
    double value;
    int row;
    int col;

    bool operator>(const Element& other) const {
        return value > other.value;
    }
};

class Matrix {
private:
    int rows;
    int cols;
    std::vector<std::vector<double>> data;

    void fillMatrix(int rows, int cols, std::vector<std::vector<double>>* A){
        std::vector<std::vector<double>>& matrix_A = *A;
        int i, j;
        for (i=0; i<rows; i++){
            for (j=0; j<cols; j++){
                matrix_A[i][j] = data[i][j];
            }
        }
    }

    static void* sortRows(void* arg) {
        ThreadData* data = static_cast<ThreadData*>(arg);
        for (int i = data->startRow; i < data->endRow; ++i) {
            std::sort((*data->matrix)[i].begin(), (*data->matrix)[i].end());
        }
        pthread_exit(nullptr);
    }

    static void sortMatrix(std::vector<std::vector<double>>& matrix) {
        if (matrix.empty()) return;

        int numRows = matrix.size();
        int numCols = matrix[0].size();
        std::priority_queue<Element, std::vector<Element>, std::greater<Element>> minHeap;

        // Initialize the heap with the first element of each row
        for (int i = 0; i < numRows; ++i) {
            if (!matrix[i].empty()) {
                minHeap.push({matrix[i][0], i, 0});
            }
        }

        std::vector<double> sortedElements;
        sortedElements.reserve(numRows * numCols);

        // Extract the smallest element and insert the next element from the same row into the heap
        while (!minHeap.empty()) {
            Element minElement = minHeap.top();
            minHeap.pop();
            sortedElements.push_back(minElement.value);

            if (minElement.col + 1 < numCols) {
                minHeap.push({matrix[minElement.row][minElement.col + 1], minElement.row, minElement.col + 1});
            }
        }

        // Reconstruct the matrix from the sorted elements
        int index = 0;
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                matrix[i][j] = sortedElements[index++];
            }
        }
    }

    static double generateRandomDouble(double n, double m) {
        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 gen(rd()); // Seed the generator
        std::uniform_real_distribution<> dis(n, m); // Define the range

        return dis(gen);
    }

public:
    Matrix(int numRows, int numCols) : rows(numRows), cols(numCols) {
        data.resize(rows);

        srand(time(nullptr));

        for (int i = 0; i < rows; ++i) {
            data[i].resize(cols);
            for (int j = 0; j < cols; ++j) {
                data[i][j] = static_cast<double>(rand()) / RAND_MAX;
            }
        }
    }
    Matrix(int numRows, int numCols, double min, double max) : rows(numRows), cols(numCols) {
        data.resize(rows);

        srand(time(nullptr));

        for (int i = 0; i < rows; ++i) {
            data[i].resize(cols);
            for (int j = 0; j < cols; ++j) {
                data[i][j] = static_cast<double>(generateRandomDouble(min, max)) / RAND_MAX;
            }
        }
    }
    Matrix(int numRows, int numCols, double value): rows(numRows), cols(numCols), data(numRows, std::vector<double>(numCols, value)) {}
    Matrix(const std::string& filePath){
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filePath << std::endl;
            return;
        }

        std::string line;
        std::getline(file, line);
        rows = std::stoi(line);
        std::getline(file, line);
        cols = std::stoi(line);

        data.resize(rows, std::vector<double>(cols, 0));

        for (int i = 0; i < rows; ++i) {
            std::getline(file, line);
            std::istringstream iss(line);
            std::string cell;
            for (int j = 0; j < cols; ++j) {
                std::getline(iss, cell, ';');
                data[i][j] = std::stod(cell);

            }
        }

        file.close();
    }

    void setValue(int row, int col, double value) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            data[row][col] = value;
        } else {
            std::cout << "Error: Invalid row or column index." << std::endl;
        }
    }
    double getValue(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            return data[row][col];
        } else {
            std::cout << "Error: Invalid row or column index." << std::endl;
            return -1; // Return a default value indicating error
        }
    }

    Matrix sortPartialySortedMatrix() {
        std::vector<std::vector<double>> A(rows, std::vector<double>(cols));

        fillMatrix(rows, cols, &A);

        sortMatrix(A);

        Matrix result(rows, cols, 0.0);   
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                result.setValue(i, j, A[i][j]);
            }
        } 
        return result;
    }

    int getRowsCount() const {
        return rows;
    }
    int getColumnsCount() const {
        return cols;
    }
    std::vector<std::vector<double>> getData() const {
        return data;
    }

    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << std::fixed << std::setprecision(6) << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void writeToFile(const std::string& filePath) const {
        std::ofstream outFile(filePath);
        
        if (!outFile.is_open()) {
            std::cerr << "Error: Unable to open file for writing: " << filePath << std::endl;
            return;
        }

        // Write the number of rows and columns in the first two rows
        outFile << rows << "\n";
        outFile << cols << "\n";

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                outFile << std::fixed << std::setprecision(6) << data[i][j];
                if (j != cols - 1) {
                    outFile << ";";
                }
            }
            outFile << "\n";
        }

        outFile.close();
    }

    Matrix sort() {

        std::vector<std::vector<double>> A(rows, std::vector<double>(cols));

        fillMatrix(rows, cols, &A);

        for (int j=0; j < rows; j++){
            std::sort(A[j].begin(), A[j].end());
        }

        Matrix result(rows, cols, 0.0);   
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                result.setValue(i, j, A[i][j]);
            }
        }  
        return result;   
    }

    Matrix sort_openm(int thread_num) {

        std::vector<std::vector<double>> A(rows, std::vector<double>(cols));

        fillMatrix(rows, cols, &A);

        #pragma omp parallel for num_threads(thread_num)
        for (int j=0; j < rows; j++){
            std::sort(A[j].begin(), A[j].end());
        }

        Matrix result(rows, cols, 0.0);   
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                result.setValue(i, j, A[i][j]);
            }
        }  
        return result;   
    }

    Matrix sort_threadlib(int thread_num) {
        std::vector<std::vector<double>> A(rows, std::vector<double>(cols));

        fillMatrix(rows, cols, &A);

        std::vector<pthread_t> threads(thread_num);
        std::vector<ThreadData> threadData(thread_num);
        int rowsPerThread = rows / thread_num;
        int extraRows = rows % thread_num;

        int currentRow = 0;
        for (int i = 0; i < thread_num; ++i) {
            int startRow = currentRow;
            int endRow = startRow + rowsPerThread + (i < extraRows ? 1 : 0);
            threadData[i] = {&A, startRow, endRow};
            currentRow = endRow;

            pthread_create(&threads[i], nullptr, sortRows, &threadData[i]);
        }

        for (int i = 0; i < thread_num; ++i) {
            pthread_join(threads[i], nullptr);
        }

        Matrix result(rows, cols, 0.0);   
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                result.setValue(i, j, A[i][j]);
            }
        }  
        return result;
    }

    Matrix complete_sort() {

        std::vector<std::vector<double>> A(rows, std::vector<double>(cols));

        fillMatrix(rows, cols, &A);

        for (int j=0; j < rows; j++){
            std::sort(A[j].begin(), A[j].end());
        }

        sortMatrix(A);

        Matrix result(rows, cols, 0.0);   
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                result.setValue(i, j, A[i][j]);
            }
        }  
        return result;   
    }

    Matrix complete_sort_openm(int thread_num) {

        std::vector<std::vector<double>> A(rows, std::vector<double>(cols));

        fillMatrix(rows, cols, &A);

        #pragma omp parallel for num_threads(thread_num)
        for (int j=0; j < rows; j++){
            std::sort(A[j].begin(), A[j].end());
        }

        sortMatrix(A);

        Matrix result(rows, cols, 0.0);   
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                result.setValue(i, j, A[i][j]);
            }
        }  
        return result;   
    }

    Matrix complete_sort_threadlib(int thread_num) {
        std::vector<std::vector<double>> A(rows, std::vector<double>(cols));

        fillMatrix(rows, cols, &A);

        std::vector<pthread_t> threads(thread_num);
        std::vector<ThreadData> threadData(thread_num);
        int rowsPerThread = rows / thread_num;
        int extraRows = rows % thread_num;

        int currentRow = 0;
        for (int i = 0; i < thread_num; ++i) {
            int startRow = currentRow;
            int endRow = startRow + rowsPerThread + (i < extraRows ? 1 : 0);
            threadData[i] = {&A, startRow, endRow};
            currentRow = endRow;

            pthread_create(&threads[i], nullptr, sortRows, &threadData[i]);
        }

        for (int i = 0; i < thread_num; ++i) {
            pthread_join(threads[i], nullptr);
        }

        sortMatrix(A);

        Matrix result(rows, cols, 0.0);   
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                result.setValue(i, j, A[i][j]);
            }
        }  
        return result;
    }


};

#endif
