#include <vector>

class Polynomial {
    private: 
        std::vector<int> coefficients;

    public:
        Polynomial(std::vector<int> coeffs) : coefficients(coeffs) {}
        virtual ~Polynomial() = default;
        std::vector<int> getCoefficients() {
            return coefficients;
        }
};
