#ifndef INTEGRAL_ALGORITHM_H
#define INTEGRAL_ALGORITHM_H

#include "Polynomial.h"

class IntegralAlgorithm {
    public:
        IntegralAlgorithm() = default;
        virtual ~IntegralAlgorithm() = default;
        virtual double integration(Polynomial& poly, double a, double b, int n) const = 0;
};

#endif
