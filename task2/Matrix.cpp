#include "Matrix.h"

Matrix::Matrix(int numRows, int numCols, double value) : rows(numRows), cols(numCols), data(numRows, std::vector<double>(numCols, value)) {}

Matrix::Matrix(int numRows, int numCols) : rows(numRows), cols(numCols) {
    data.resize(rows);

    srand(time(nullptr));

    for (int i = 0; i < rows; ++i) {
        data[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            data[i][j] = static_cast<double>(rand()) / RAND_MAX;
        }
    }
}

Matrix::Matrix(const std::string& filePath) {
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

void Matrix::setValue(int row, int col, double value) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        data[row][col] = value;
    } else {
        std::cout << "Error: Invalid row or column index." << std::endl;
    }
}

double Matrix::getValue(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return data[row][col];
    } else {
        std::cout << "Error: Invalid row or column index." << std::endl;
        return -1; // Return a default value indicating error
    }
}

int Matrix::getRowsCount() const {
    return rows;
}

int Matrix::getColumnsCount() const {
    return cols;
}

void Matrix::print() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << std::fixed << std::setprecision(6) << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::operator*(const Matrix& other) const {
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

Matrix Matrix::matrixMultiply(const Matrix& other, int numThread) const {
    if (cols != other.rows) {
        std::cerr << "Error: Matrix dimensions mismatch for multiplication" << std::endl;
        return Matrix(0, 0);
    }

    Matrix result(rows, other.cols, 0.0);
    #pragma omp parallel for collapse(2) schedule(static) num_threads(numThread)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            for (int k = 0; k < cols; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }

    return result;
}

void Matrix::writeToFile(const std::string& filePath) const {
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
