#include "Polynomial.cpp"

class QuadraticPolynomial : public Polynomial {
    private:
        int quadraticCoefficient;
        int linearCoefficient;
        int constant;

    public:
        QuadraticPolynomial(int a, int b, int c) {
            quadraticCoefficient = a;
            linearCoefficient = b;
            constant = c;
        }

        int getQuadraticCoefficient() {
            return quadraticCoefficient;
        }

        int getLinearCoefficient() {
            return linearCoefficient;
        }

        int getConstant() {
            return constant;
        }
};
