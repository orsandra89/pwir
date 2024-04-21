#include "SimpsonAlgorithmMPI.h"

double SimpsonAlgorithmMPI::integration(Polynomial& poly, double a, double b, int n) const {
    MPI_Init(NULL, NULL);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double h = (b - a) / n;
    int local_n = n / size;
    double local_a = a + rank * local_n * h;
    double local_b = local_a + local_n * h;

    double local_sum = 0.0;
    for (int i = 0; i <= local_n; ++i) {
        double x = local_a + i * h;
        double coeff = (i == 0 || i == local_n) ? 1 : (i % 2 == 0 ? 2 : 4);
        local_sum += coeff * poly.evaluate(x);
    }

    double total_sum;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        return total_sum * h / 3.0;
    } else {
        return 0.0; // Non-root processes return 0
    }
}
