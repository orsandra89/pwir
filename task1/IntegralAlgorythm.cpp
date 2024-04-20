#include "Polynomial.cpp"

class IntegralAlgorythm {
    public: 
        virtual ~IntegralAlgorythm() = default;
        virtual double integration(Polynomial f, int a, int b, int n);
};
