#ifndef RECTANGLE_ALGORITHM_H
#define RECTANGLE_ALGORITHM_H

#include <iostream>
#include "IntegralAlgorithm.h"

class RectangleAlgorithm : public IntegralAlgorithm {
    public:
        double integration(Polynomial& poly, double a, double b, int n) const override;
};

#endif