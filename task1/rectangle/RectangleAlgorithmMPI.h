#ifndef RECTANGLE_ALGORITHM_MPI_H
#define RECTANGLE_ALGORITHM_MPI_H

#include "../IntegralAlgorithm.h"
#include <mpi.h>
#include <iostream>

class RectangleAlgorithmMPI : public IntegralAlgorithm {
public:
    double integration(Polynomial& poly, double a, double b, int n) const override;
};

#endif
