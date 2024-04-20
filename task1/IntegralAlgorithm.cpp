#include "Polynomial.cpp"

class IntegralAlgorithm {
    public: 
        virtual ~IntegralAlgorithm() = default;
        virtual double integration(Polynomial f, int a, int b, int n);
};
