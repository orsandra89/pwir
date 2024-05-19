#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <string>
#include <iostream>

class Polynomial {
    private:
        std::vector<double> coefficients;

    public:
        Polynomial();
        Polynomial(std::vector<double> coeffs);
        ~Polynomial() = default;
        double evaluate(double x) const;
        void print() const;
};

#endif
