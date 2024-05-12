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

class Matrix {
private:
    int rows;
    int cols;
    std::vector<std::vector<double>> data;

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
        cols = rows + 1;

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

    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            std::cerr << "Error: Matrix dimensions mismatch for multiplication" << std::endl;
            return Matrix(0, 0, 0); // Return an empty matrix
        }

        Matrix result(rows, other.cols, 0.0);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                for (int k = 0; k < cols; ++k) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }

        return result;
    }
    Matrix matrixMultiply(const Matrix& other, int numThread) const {
        if (cols != other.rows) {
            std::cerr << "Error: Matrix dimensions mismatch for multiplication" << std::endl;
            return Matrix(0, 0);
        }

        Matrix result(rows, other.cols, 0.0);
        #pragma omp parallel for collapse(2) schedule(static) num_threads(numThread)
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                double sum = 0.0;
                #pragma omp parallel for reduction(+:sum)
                for (int k = 0; k < cols; ++k) {
                    sum += data[i][k] * other.data[k][j];
                }
                result.data[i][j] = sum;
            }
        }

        return result;
    }
    Matrix matrixMultiplyThreadLibrary(const Matrix& other, int numThread) const {
        if (cols != other.rows) {
            std::cerr << "Error: Matrix dimensions mismatch for multiplication" << std::endl;
            return Matrix(0, 0);
        }

        Matrix result(rows, other.cols, 0.0);

        auto compute_cell = [&](int row, int col) {
            double sum = 0;
            for (int k = 0; k < cols; ++k) {
                sum += data[row][k] * other.data[k][col];
            }
            result.data[row][col] = sum;
        };

        // Create threads and perform multiplication
        std::vector<std::thread> threads;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                if (threads.size() < numThread) {
                    threads.emplace_back(compute_cell, i, j);
                } else {
                    for (auto& t : threads) {
                        t.join();
                    }
                    threads.clear();
                    threads.emplace_back(compute_cell, i, j);
                }
            }
        }

        // Join remaining threads
        for (auto& t : threads) {
            t.join();
        }

        return result;
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

    std::vector<double> gauss_elimination() {
        if (rows != cols - 1) {
            std::cerr << "Error: Matrix dimensions mismatch for Gauss elimination" << std::endl;
            return;
        }

        int n = rows;

        // Forward Elimination
        for (int i = 0; i < n; ++i) {
            // Partial pivoting
            int max_row = i;
            for (int j = i + 1; j < n; ++j) {
                if (abs(matrix[j][i]) > abs(matrix[max_row][i])) {
                    max_row = j;
                }
            }
            swap(matrix[i], matrix[max_row]);

            // Make all rows below this one 0 in the current column
            for (int j = i + 1; j < n; ++j) {
                double factor = matrix[j][i] / matrix[i][i];
                for (int k = i; k <= n; ++k) {
                    matrix[j][k] -= factor * matrix[i][k];
                }
            }
        }

        // Back Substitution
        std::vector<double> x(n);
        for (int i = n - 1; i >= 0; --i) {
            x[i] = matrix[i][n] / matrix[i][i];
            for (int k = i - 1; k >= 0; --k) {
                matrix[k][n] -= matrix[k][i] * x[i];
            }
        }
        return x;
    }
};

#endif
