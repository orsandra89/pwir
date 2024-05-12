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

struct thread_param{
    int norm;
    int i;
    int N;
    std::vector<std::vector<double>>* A;
    std::vector<double>* b;
    int num_threads;
};

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

    void completePivot(int n, std::vector<std::vector<double>>* a, std::vector<double>* b, std::vector<int>* order, int j){
        std::vector<std::vector<double>>& matrix_A = *a;
        std::vector<double>& vector_b = *b;
        std::vector<int>& vector_order = *order;

        int   i,k,rowx;
        double xfac, amax;

        amax = (double) fabs(matrix_A[j][j]) ;
        int max_row = j;
        int max_col = j;
        for (i=j+1; i<n; i++){   /* Find the row with largest pivot */
            for (k = j; k < n; ++k) {
                xfac = (double) fabs(matrix_A[i][k]);
                if(xfac > amax) {amax = xfac; max_row=i; max_col=k;}
            }
        }

        if(max_row != j || max_col != j) {  /* Row interchanges */
            rowx = rowx+1;
            std::swap(vector_b[j], vector_b[max_row]);
            std::swap(vector_order[j], vector_order[max_col]);
            for (k = 0; k < n; ++k) {
                std::swap(matrix_A[j][k], matrix_A[max_row][k]);
            }
            for (i = 0; i < n; ++i) {
                std::swap(matrix_A[i][j], matrix_A[i][max_col]);
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

    static void *inner_loop(void * param){
        //int norm = *((int *) param);
        struct thread_param* tparam = (struct thread_param*) param;
        int N = tparam->N;
        int num_threads = tparam->num_threads;
        std::vector<std::vector<double>>& matrix_A = *tparam->A;
        std::vector<double>& vector_b = *tparam->b;
        int norm = tparam->norm;
        int i = tparam->i;
        //printf("thread = %d\n", norm);
        float multiplier;
        int row, col;
        for (row = norm + i + 1; row < N; row = row + num_threads) {
            multiplier = matrix_A[row][norm] / matrix_A[norm][norm];
            for (col = norm; col < N; col++) {
                matrix_A[row][col] -= matrix_A[norm][col] * multiplier;
            }
            vector_b[row] -= vector_b[norm] * multiplier;
        }
        pthread_exit(0);
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

    std::vector<double> gauss_elimination(bool complete_pivoting) {
        if (rows != cols - 1) {
            std::cerr << "Error: Matrix dimensions mismatch for Gauss elimination" << std::endl;
            return std::vector<double> {0.0};
        }

        int N = data.size();

        std::vector<int> order;
        for (int i = 0; i < N; ++i) {
            order.push_back(i);
        }
        std::vector<std::vector<double>> A(N, std::vector<double>(N));
        std::vector<double> b(N);
        std::vector<double> x(N);

        fillMatrix(N, &A, &b); //Fill in matrix A and vector B with values from Matrix

        int i, k;
        double m;

        //Gaussian Elimination
        for (int j=0; j < N-1; j++){
            if (complete_pivoting) {
                completePivot(N, &A, &b, &order, j);
            } else {
                partialPivot(N, &A, &b, j);
            }
                
            for (int k=j+1; k<N; k++){
                m = A[k][j]/A[j][j];
                for (i=j; i<N; i++){
                    A[k][i] = A[k][i] - (m * A[j][i]);
                }
                b[k] = b[k] - (m * b[j]);
            }

        }

        backSubstitution(N, &A, &b, &x, 1);

        if(complete_pivoting) {
            std::vector<double> result(N);
            int o = 0;
            for(int item : order){
                result[item] = x[o];
                o++;
            }
            return result;
        } else {
            return x;
        }
    }

    std::vector<double> gauss_elimination_openmp(int numThreads, bool complete_pivoting) {
        if (rows != cols - 1) {
            std::cerr << "Error: Matrix dimensions mismatch for Gauss elimination" << std::endl;
            return std::vector<double> {0.0};
        }

        int N = data.size();

        std::vector<int> order;
        for (int i = 0; i < N; ++i) {
            order.push_back(i);
        }
        std::vector<std::vector<double>> A(N, std::vector<double>(N));
        std::vector<double> b(N);
        std::vector<double> x(N);

        fillMatrix(N, &A, &b); //Fill in matrix A and vector B with values from Matrix

        int i, k;
        double m;

        //Gaussian Elimination
        for (int j=0; j < N-1; j++){
            if (complete_pivoting) {
                completePivot(N, &A, &b, &order, j);
            } else {
                partialPivot(N, &A, &b, j);
            }

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

        if(complete_pivoting) {
            std::vector<double> result(N);
            int o = 0;
            for(int item : order){
                result[item] = x[o];
                o++;
            }
            return result;
        } else {
            return x;
        }
    }

    std::vector<double> gauss_elimination_threadlib(int numThreads, bool complete_pivoting) {
        if (rows != cols - 1) {
            std::cerr << "Error: Matrix dimensions mismatch for Gauss elimination" << std::endl;
            return std::vector<double> {0.0};
        }

        int N = data.size();

        std::vector<int> order;
        for (int i = 0; i < N; ++i) {
            order.push_back(i);
        }
        std::vector<std::vector<double>> A(N, std::vector<double>(N));
        std::vector<double> b(N);
        std::vector<double> x(N);

        fillMatrix(N, &A, &b); //Fill in matrix A and vector B with values from Matrix

        int norm, row, col;  /* Normalization row, and zeroing
			* element row and col */
        double multiplier;

        pthread_t thread[N];

        printf("Computing Serially.\n");

        /* Gaussian elimination */
        for (norm = 0; norm < N - 1; norm++) {
            struct thread_param* param = (thread_param*) malloc(numThreads * sizeof(struct thread_param));
            if ( param == NULL ) {
                fprintf(stderr, "Couldn't allocate memory for thread.\n");
                exit(EXIT_FAILURE);
            }

            if (complete_pivoting) {
                completePivot(N, &A, &b, &order, norm);
            } else {
                partialPivot(N, &A, &b, norm);
            }

            int i, j;

            for(i = 0; i < numThreads; i++){
                param[i].norm = norm;
                param[i].i = i;
                param[i].N = N;
                param[i].num_threads = numThreads;
                param[i].A = &A;
                param[i].b = &b;
                pthread_create(&thread[i], NULL, inner_loop, (void*) &param[i]);
            }

            for (j = 0; j < numThreads; j++) {
                pthread_join(thread[j], NULL);
            }

            free(param);
        }

        backSubstitution(N, &A, &b, &x, numThreads);

        if(complete_pivoting) {
            std::vector<double> result(N);
            int o = 0;
            for(int item : order){
                result[item] = x[o];
                o++;
            }
            return result;
        } else {
            return x;
        }
    }

    std::vector<double> gauss_jourdan_elimination(bool complete_pivoting) {
        if (rows != cols - 1) {
            std::cerr << "Error: Matrix dimensions mismatch for Gauss elimination" << std::endl;
            return std::vector<double> {0.0};
        }

        int N = data.size();

        std::vector<std::vector<double>> A(N, std::vector<double>(N+1));

        for (int i=0; i<N; i++){
            for (int j=0; j<N+1; j++){
                A[i][j] = data[i][j];
            }
        }

        int i, k;
        double m;

        for (int i = 0; i < N; ++i) {
            // Make the diagonal element non-zero
            if (A[i][i] == 0) {
                for (int k = i + 1; k < N; ++k) {
                    if (A[k][i] != 0) {
                        std::swap(A[i], A[k]);
                        break;
                    }
                }
            }

            // Make the diagonal element 1
            double factor = A[i][i];
            for (int k = i; k < N + 1; ++k) {
                A[i][k] /= factor;
            }

            // Eliminate elements above and below the diagonal
            for (int j = 0; j < N; ++j) {
                if (j != i) {
                    factor = A[j][i];
                    for (int k = i; k < N + 1; ++k) {
                        A[j][k] -= factor * A[i][k];
                    }
                }
            }
        }

        std::vector<double> result(N);
        for(int i = 0; i < N; i++){
            result[i] = A[i][N];
        }
        return result;
    }

    std::vector<double> gauss_jourdan_elimination_openmp(int num_threads, bool complete_pivoting) {
        if (rows != cols - 1) {
            std::cerr << "Error: Matrix dimensions mismatch for Gauss elimination" << std::endl;
            return std::vector<double> {0.0};
        }

        int N = data.size();

        std::vector<std::vector<double>> A(N, std::vector<double>(N+1));

        for (int i=0; i<N; i++){
            for (int j=0; j<N+1; j++){
                A[i][j] = data[i][j];
            }
        }

        int i, k, j;
        double m, temp;

        for (int i = 0; i < N; ++i) {
            // Make the diagonal element non-zero
            if (A[i][i] == 0) {
                for (int k = i + 1; k < N; ++k) {
                    if (A[k][i] != 0) {
                        std::swap(A[i], A[k]);
                        break;
                    }
                }
            }

            #pragma omp parallel for num_threads(num_threads) private(temp,k,j) 
            for(k=i+1; k < N; k++){
                temp = A[k][i]/A[i][i];
                for(j=i;j<=N;j++){
                    A[k][j] = A[k][j] - ( temp * A[i][j]);
                }			
            }
        }

        for(k=N-1;k>0;k--){
            #pragma omp parallel for num_threads(num_threads) private(i)	
            for(i=0; i<k; i++){
                A[i][N] = A[i][N] - ((A[i][k]/A[k][k]) * A[k][N]);
                A[i][k] = 0;
            }
        }

        std::vector<double> result(N);
        #pragma omp parallel for num_threads(num_threads) private(i)
        for(int i = 0; i < N; i++){
            result[i] = A[i][N]/A[i][i];
        }
        return result;
    }

};

#endif
