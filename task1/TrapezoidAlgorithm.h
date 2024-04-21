#ifndef TRAPEZOID_ALGORITHM_H
#define TRAPEZOID_ALGORITHM_H

#include "IntegralAlgorithm.h"

class TrapezoidAlgorithm : public IntegralAlgorithm {
public:
    double integration(Polynomial& poly, double a, double b, int n) const override;
};

#endif // TRAPEZOID_ALGORITHM_H
