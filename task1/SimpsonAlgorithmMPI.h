#ifndef SIMPSON_ALGORITHM_MPI_H
#define SIMPSON_ALGORITHM_MPI_H

#include "IntegralAlgorithm.h"
#include <mpi.h>

class SimpsonAlgorithmMPI : public IntegralAlgorithm {
public:
    double integration(Polynomial& poly, double a, double b, int n) const override;
};

#endif
