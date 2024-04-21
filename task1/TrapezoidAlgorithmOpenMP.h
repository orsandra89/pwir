#ifndef TRAPEZOID_ALGORITHM_OPENMP_H
#define TRAPEZOID_ALGORITHM_OPENMP_H

#include "IntegralAlgorithm.h"

class TrapezoidAlgorithmOpenMP : public IntegralAlgorithm {
public:
    double integration(Polynomial& poly, double a, double b, int n) const override;
};

#endif // TRAPEZOID_ALGORITHM_OPENMP_H
