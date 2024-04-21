#include "TrapezoidAlgorithmMPI.h"

double TrapezoidAlgorithmMPI::integration(Polynomial& poly, double a, double b, int n) const {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double h = (b - a) / static_cast<double>(n);
    int local_n = n / size;
    double local_a = a + rank * local_n * h;
    double local_b = local_a + local_n * h;

    double local_sum = 0.0;
    for (int i = 0; i < local_n; ++i) {
        double x1 = local_a + i * h;
        double x2 = local_a + (i + 1) * h;
        local_sum += (poly.evaluate(x1) + poly.evaluate(x2)) * h / 2.0;
    }

    double total_sum;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        return total_sum * h / 3.0;
    } else {
        return 0.0; // Non-root processes return 0
    }
}
