#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include "polynomial/Polynomial.h"
#include "polynomial/QuadraticPolynomial.h"
#include "IntegralAlgorithm.h"
#include "simpson/SimpsonAlgorithm.h"
#include "rectangle/RectangleAlgorithm.h"
#include "trapezoid/TrapezoidAlgorithm.h"
#include "simpson/SimpsonAlgorithmOpenMP.h"
#include "rectangle/RectangleAlgorithmOpenMP.h"
#include "trapezoid/TrapezoidAlgorithmOpenMP.h"

// Function to get user input for a double value
double getDoubleInput(const std::string& prompt) {
    double value;
    std::cout << prompt;
    std::cin >> value;
    return value;
}

// Function to swap two double values if the second one is smaller than the first one
void swapIfGreater(double& a, double& b) {
    if (b < a) {
        std::swap(a, b);
    }
}

struct ThreadData {
    Polynomial poly;
    double start;
    double end;
    int steps;
    double result;
};

void* integrate(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    Polynomial poly = data->poly;
    double step = (data->end - data->start) / data->steps;
    double sum = 0.0;
    for (int i = 0; i < data->steps; ++i) {
        double x = data->start + (i + 0.5) * step;
        sum += poly.evaluate(x) * step;
    }
    data->result = sum;
    return nullptr;
}

double rectangle_thread_lib(Polynomial p, double start, double end, int steps, int num_threads) {
    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> threadData(num_threads);

    double segment = (end - start) / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        threadData[i].poly = p;
        threadData[i].start = start + i * segment;
        threadData[i].end = start + (i + 1) * segment;
        threadData[i].steps = steps / num_threads;
        pthread_create(&threads[i], nullptr, integrate, &threadData[i]);
    }

    double total_integral = 0.0;
    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
        total_integral += threadData[i].result;
    }

    return total_integral;
}

int main(int argc, char *argv[]) {
    // Mapping of integers to algorithm names
    std::map<int, std::string> algorithmMap = {
        {0, "Rectangle"},
        {1, "RectangleThreadLib"},
        {2, "RectangleOpenMP"},
        {3, "Trapezoid"},
        {4, "TrapezoidOpenMP"},
        {5, "Simpson"},
        {6, "SimpsonOpenMP"}
    };

    if (argc < 8) {
        std::cout << "Available integration algorithms:" << std::endl;
        for (const auto& pair : algorithmMap) {
            std::cout << pair.first << ". " << pair.second << std::endl;
        }
        std::cerr << "Usage: " << argv[0] << " <A> <B> <C> <a> <b> <n> <algorithm> <thread_num>" << std::endl;
        return 1;
    }

    double A = std::stod(argv[1]);
    double B = std::stod(argv[2]);
    double C = std::stod(argv[3]);
    double a = std::stod(argv[4]);
    double b = std::stod(argv[5]);
    int n = std::stoi(argv[6]);
    int choice = std::stoi(argv[7]);

    QuadraticPolynomial poly(A, B, C);

    auto it = algorithmMap.find(choice);
    if (it == algorithmMap.end()) {
        std::cerr << "Invalid algorithm choice. Please try again." << std::endl;
        return 0;
    }

    std::string algorithm = it->second;

    IntegralAlgorithm* integrationAlgorithm;
    if (algorithm == "Simpson") {
        integrationAlgorithm = new SimpsonAlgorithm();

        auto start = std::chrono::steady_clock::now();

        double integral = integrationAlgorithm->integration(poly, a, b, n);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result of integration: " << integral << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;
    } else if (algorithm == "Rectangle") {
        integrationAlgorithm = new RectangleAlgorithm();

        auto start = std::chrono::steady_clock::now();

        double integral = integrationAlgorithm->integration(poly, a, b, n);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result of integration: " << integral << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;
    } else if (algorithm == "Trapezoid") {
        integrationAlgorithm = new TrapezoidAlgorithm();

        auto start = std::chrono::steady_clock::now();

        double integral = integrationAlgorithm->integration(poly, a, b, n);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result of integration: " << integral << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;
    } else if (algorithm == "SimpsonOpenMP") {
        integrationAlgorithm = new SimpsonAlgorithmOpenMP();

        auto start = std::chrono::steady_clock::now();

        double integral = integrationAlgorithm->integration(poly, a, b, n);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result of integration: " << integral << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;
    } else if (algorithm == "RectangleOpenMP") {
        integrationAlgorithm = new RectangleAlgorithmOpenMP();

        auto start = std::chrono::steady_clock::now();

        double integral = integrationAlgorithm->integration(poly, a, b, n);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result of integration: " << integral << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;
    } else if (algorithm == "TrapezoidOpenMP") {
        integrationAlgorithm = new TrapezoidAlgorithmOpenMP();

        auto start = std::chrono::steady_clock::now();

        double integral = integrationAlgorithm->integration(poly, a, b, n);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result of integration: " << integral << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;

    delete integrationAlgorithm;
    } else if (algorithm == "RectangleThreadLib") {
        if (argc != 9) {
            std::cout << "Available integration algorithms:" << std::endl;
            for (const auto& pair : algorithmMap) {
                std::cout << pair.first << ". " << pair.second << std::endl;
            }
            std::cerr << "Usage: " << argv[0] << " <A> <B> <C> <a> <b> <n> <algorithm> <thread_num>" << std::endl;
            return 1;
        }
        int num_threads = std::stoi(argv[8]);

        auto start = std::chrono::steady_clock::now();
        
        double integral = rectangle_thread_lib(poly, a, b, n, num_threads);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Result of integration: " << integral << std::endl;

        std::cout << "Calculation time: " << elapsed_time << std::endl;
    } else {
        std::cerr << "Invalid algorithm choice." << std::endl;

        std::cout << "Available integration algorithms:" << std::endl;
        for (const auto& pair : algorithmMap) {
            std::cout << pair.first << ". " << pair.second << std::endl;
        }

        return 0;
    }

    return 0;
}
