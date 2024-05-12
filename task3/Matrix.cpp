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

    void fillMatrix(int N, std::vector<std::vector<double>>* A, std::vector<double>* b){
        std::vector<std::vector<double>>& matrix_A = *A;
        std::vector<double>& vector_b = *b;
        int i, j;
        for (i=0; i<N; i++){
            for (j=0; j<N; j++){
                matrix_A[i][j] = data[i][j];
            }
            vector_b[i] = data[i][N];
        }
    }

    void partialPivot(int n, std::vector<std::vector<double>>* a, std::vector<double>* b, int j){
        std::vector<std::vector<double>>& matrix_A = *a;
        std::vector<double>& vector_b = *b;

        int   i,k,m,rowx;
        double xfac, amax;

        amax = (double) fabs(matrix_A[j][j]) ;
        m = j;
        for (i=j+1; i<n; i++){   /* Find the row with largest pivot */
            xfac = (double) fabs(matrix_A[i][j]);
            if(xfac > amax) {amax = xfac; m=i;}
        }

        if(m != j) {  /* Row interchanges */
            rowx = rowx+1;
            std::swap(vector_b[j], vector_b[m]);
            for(k=j; k<n; k++) {
                std::swap(matrix_A[j][k], matrix_A[m][k]);
            }
        }
    }

    void backSubstitution(int N, std::vector<std::vector<double>>* A, std::vector<double>* b, std::vector<double>* x, int numThreads){
        std::vector<std::vector<double>>& matrix_A = *A;
        std::vector<double>& vector_b = *b;
        std::vector<double>& vector_x = *x;
        
        int i,j;
        for (i=N-1; i >= 0; i--){
            vector_x[i] = vector_b[i];
            for (j=i+1; j<N; j++){
                vector_x[i] -= matrix_A[i][j]*vector_x[j];
            }
            vector_x[i] = vector_x[i] / matrix_A[i][i];
        }
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
            return std::vector<double> {0.0};
        }

        int N = data.size();
        std::vector<std::vector<double>> A(N, std::vector<double>(N));
        std::vector<double> b(N);
        std::vector<double> x(N);

        fillMatrix(N, &A, &b); //Fill in matrix A and vector B with values from Matrix

        int i, k;
        double m;

        //Gaussian Elimination
        for (int j=0; j < N-1; j++){
            partialPivot(N, &A, &b, j);
                
            for (int k=j+1; k<N; k++){
                m = A[k][j]/A[j][j];
                for (i=j; i<N; i++){
                    A[k][i] = A[k][i] - (m * A[j][i]);
                }
                b[k] = b[k] - (m * b[j]);
            }

        }

        backSubstitution(N, &A, &b, &x, 1);

        return x;
    }

    std::vector<double> gauss_elimination_openmp(int numThreads) {
        if (rows != cols - 1) {
            std::cerr << "Error: Matrix dimensions mismatch for Gauss elimination" << std::endl;
            return std::vector<double> {0.0};
        }

        int N = data.size();
        std::vector<std::vector<double>> A(N, std::vector<double>(N));
        std::vector<double> b(N);
        std::vector<double> x(N);

        fillMatrix(N, &A, &b); //Fill in matrix A and vector B with values from Matrix

        int i, k;
        double m;

        //Gaussian Elimination
        for (int j=0; j < N-1; j++){
            partialPivot(N, &A, &b, j);

            #pragma omp parallel default(none) num_threads(numThreads) shared(N,A,b,j) private(i,k,m)
            #pragma omp for schedule(static)
                
            for (int k=j+1; k<N; k++){
                m = A[k][j]/A[j][j];
                for (i=j; i<N; i++){
                    A[k][i] = A[k][i] - (m * A[j][i]);
                }
                b[k] = b[k] - (m * b[j]);
            }

        }

        backSubstitution(N, &A, &b, &x, numThreads);

        return x;
    }

};

#endif
