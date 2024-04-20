#include "Polynomial.cpp"

class QuadraticPolynomial : public Polynomial {
    public:
        QuadraticPolynomial(int a, int b, int c) : Polynomial({c, b, a}) {}
};
