#include <iostream>
#include <vector>
#include <mpi.h>
#include "Matrix.cpp"

std::vector<double> flatten(const std::vector<std::vector<double>>& matrix) {
    std::vector<double> flattened;
    for (const auto& row : matrix) {
        flattened.insert(flattened.end(), row.begin(), row.end());
    }
    return flattened;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 4) {
        if (rank == 0) {
            std::cerr << "Usage: " << argv[0] << " matrix1_file matrix2_file result_file" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Load matrices from files
        Matrix mat1(argv[1]);
        Matrix mat2(argv[2]);
        int rows = mat1.getRowsCount();
        int cols = mat2.getColumnsCount();
        Matrix result(rows, cols, 0.0);

        // Scatter data to other processes
        std::vector<double> mat1_data = flatten(mat1.getData());
        std::vector<double> mat2_data = flatten(mat2.getData());
        MPI_Scatter(mat1_data.data(), rows * cols / size, MPI_DOUBLE, MPI_IN_PLACE, rows * cols / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(mat2_data.data(), rows * cols / size, MPI_DOUBLE, MPI_IN_PLACE, rows * cols / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Perform local matrix multiplication
        std::vector<double> local_result(rows * cols / size, 0);
        for (int i = 0; i < rows / size; ++i) {
            for (int j = 0; j < cols; ++j) {
                for (int k = 0; k < rows; ++k) {
                    local_result[i * cols + j] += mat1_data[i * rows + k] * mat2_data[k * cols + j];
                }
            }
        }

        // Gather local results
        MPI_Gather(local_result.data(), rows * cols / size, MPI_DOUBLE, result.getData().data(), rows * cols / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Save result to file
        result.writeToFile(argv[3]);
    } else {
        // Receive scattered data
        int rows, cols;
        MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
        std::vector<double> mat1_data(rows * cols / size);
        std::vector<double> mat2_data(rows * cols / size);
        MPI_Scatter(nullptr, rows * cols / size, MPI_DOUBLE, mat1_data.data(), rows * cols / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(nullptr, rows * cols / size, MPI_DOUBLE, mat2_data.data(), rows * cols / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Perform local matrix multiplication
        std::vector<double> local_result(rows * cols / size, 0);
        for (int i = 0; i < rows / size; ++i) {
            for (int j = 0; j < cols; ++j) {
                for (int k = 0; k < rows; ++k) {
                    local_result[i * cols + j] += mat1_data[i * rows + k] * mat2_data[k * cols + j];
                }
            }
        }

        // Gather local results
        MPI_Gather(local_result.data(), rows * cols / size, MPI_DOUBLE, nullptr, rows * cols / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}