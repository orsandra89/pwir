#include "RectangleAlgorithmMPI.h"

double RectangleAlgorithmMPI::integration(Polynomial& poly, double a, double b, int n) const {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    double local_integral = 0.0;
        // Calculate local interval for each process
    int local_num_intervals = n / size;
    double local_a = a + rank * (b - a) / size;
    double local_b = local_a + (b - a) / size;

    double interval_width = (b - a) / n;
    for (int i = 0; i < local_num_intervals; ++i) {
        double x_mid = local_a + (i + 0.5) * interval_width;
        local_integral += poly.evaluate(x_mid) * interval_width;
    }

    double total_integral;
    MPI_Reduce(&local_integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        return total_integral;
    } else {
        return 0.0;
    }
}
