#ifndef SIMPSON_ALGORITHM_H
#define SIMPSON_ALGORITHM_H

#include "../IntegralAlgorithm.h"

class SimpsonAlgorithm : public IntegralAlgorithm {
public:
    double integration(Polynomial& poly, double a, double b, int n) const override;
};

#endif
