#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Matrix {
private:
    int rows;
    int cols;
    std::vector<std::vector<double>> data;

public:
    // Constructor
    Matrix(int numRows, int numCols);
    Matrix(const std::string& filePath);

    // Method to set value at specific position
    void setValue(int row, int col, double value);

    // Method to get value at specific position
    double getValue(int row, int col) const;

    // Method to print the matrix
    void print() const;

    Matrix operator*(const Matrix& other) const;
};

#endif
