#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>

class Polynomial {
    private:
        std::vector<double> coefficients;

    public:
        Polynomial(std::vector<double> coeffs);
        ~Polynomial() = default;
        double evaluate(double x) const;
};

#endif
