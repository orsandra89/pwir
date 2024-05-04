#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

class Matrix {
private:
    int rows;
    int cols;
    std::vector<std::vector<double>> data;

public:
    Matrix(int numRows, int numCols);
    Matrix(int numRows, int numCols, double value);
    Matrix(const std::string& filePath);

    void setValue(int row, int col, double value);
    double getValue(int row, int col) const;

    int getRowsCount() const;
    int getColumnsCount() const;

    void print() const;

    Matrix operator*(const Matrix& other) const;
    Matrix matrixMultiply(const Matrix& other, int numThread) const;

    void writeToFile(const std::string& filePath) const;
};

#endif
