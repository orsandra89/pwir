#ifndef RECTANGLE_ALGORITHM_OPENMP_H
#define RECTANGLE_ALGORITHM_OPENMP_H

#include "IntegralAlgorithm.h"
#include <omp.h>

class RectangleAlgorithmOpenMP : public IntegralAlgorithm {
    public:
        double integration(Polynomial& poly, double a, double b, int n) const override;
};

#endif // RECTANGLE_ALGORITHM_OPENMP_H