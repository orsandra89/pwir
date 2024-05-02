#include "Matrix.h"

// Constructor
Matrix::Matrix(int numRows, int numCols) : rows(numRows), cols(numCols), data(numRows, std::vector<double>(numCols, 0)) {}

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

// Method to set value at specific position
void Matrix::setValue(int row, int col, double value) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        data[row][col] = value;
    } else {
        std::cout << "Error: Invalid row or column index." << std::endl;
    }
}

// Method to get value at specific position
double Matrix::getValue(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return data[row][col];
    } else {
        std::cout << "Error: Invalid row or column index." << std::endl;
        return -1; // Return a default value indicating error
    }
}

// Method to print the matrix
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
        return Matrix(0, 0); // Return an empty matrix
    }

    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            for (int k = 0; k < cols; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }

    return result;
}
