#ifndef TRAPEZOID_ALGORITHM_MPI_H
#define TRAPEZOID_ALGORITHM_MPI_H

#include "../IntegralAlgorithm.h"
#include <mpi.h>

class TrapezoidAlgorithmMPI : public IntegralAlgorithm {
public:
    double integration(Polynomial& poly, double a, double b, int n) const override;
};

#endif
