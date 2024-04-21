#ifndef SIMPSON_ALGORITHM_OPENMP_H
#define SIMPSON_ALGORITHM_OPENMP_H

#include "../IntegralAlgorithm.h"

class SimpsonAlgorithmOpenMP : public IntegralAlgorithm {
public:
    double integration(Polynomial& poly, double a, double b, int n) const override;
};

#endif
