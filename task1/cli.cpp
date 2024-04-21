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

int main() {
    // Mapping of integers to algorithm names
    std::map<int, std::string> algorithmMap = {
        {0, "Exit"},
        {1, "Rectangle"},
        {2, "RectangleOpenMP"},
        {3, "Trapezoid"},
        {4, "TrapezoidOpenMP"},
        {5, "Simpson"},
        {6, "SimpsonOpenMP"}
    };

    while (true) {
        // Display available algorithms
        std::cout << "Available integration algorithms:" << std::endl;
        for (const auto& pair : algorithmMap) {
            std::cout << pair.first << ". " << pair.second << std::endl;
        }

        // Prompt the user to choose an integration algorithm
        int choice;
        std::cout << "Choose an integration algorithm (enter the corresponding number, 0 to exit): ";
        std::cin >> choice;

        // Check if the user wants to exit
        if (choice == 0) {
            std::cout << "Exiting the program." << std::endl;
            break;
        }

        // Validate user choice
        auto it = algorithmMap.find(choice);
        if (it == algorithmMap.end()) {
            std::cerr << "Invalid algorithm choice. Please try again." << std::endl;
            continue; // Ask the user to choose again
        }
        std::string algorithm = it->second;

        // Create an instance of the chosen algorithm
        IntegralAlgorithm* integrationAlgorithm;
        if (algorithm == "Simpson") {
            integrationAlgorithm = new SimpsonAlgorithm();
        } else if (algorithm == "Rectangle") {
            integrationAlgorithm = new RectangleAlgorithm();
        } else if (algorithm == "Trapezoid") {
            integrationAlgorithm = new TrapezoidAlgorithm();
        } else if (algorithm == "SimpsonOpenMP") {
            integrationAlgorithm = new SimpsonAlgorithmOpenMP();
        } else if (algorithm == "RectangleOpenMP") {
            integrationAlgorithm = new RectangleAlgorithmOpenMP();
        } else if (algorithm == "TrapezoidOpenMP") {
            integrationAlgorithm = new TrapezoidAlgorithmOpenMP();
        } else {
            std::cerr << "Invalid algorithm choice. Please try again." << std::endl;
            continue; // Ask the user to choose again
        }

        // Get coefficients of the quadratic polynomial
        double A = getDoubleInput("Enter coefficient A: ");
        double B = getDoubleInput("Enter coefficient B: ");
        double C = getDoubleInput("Enter coefficient C: ");

        // Get interval [a, b]
        double a = getDoubleInput("Enter interval start (a): ");
        double b = getDoubleInput("Enter interval end (b): ");
        swapIfGreater(a, b); // Swap a and b if b < a

        // Create a Polynomial object with the given coefficients
        QuadraticPolynomial poly(A, B, C);

        // Perform integration using the chosen algorithm
        double num_intervals = getDoubleInput("Enter number of intervals (n): ");
        double integral = integrationAlgorithm->integration(poly, a, b, num_intervals);

        // Output the result
        std::cout << "Result of integration: " << integral << std::endl;

        // Clean up
        delete integrationAlgorithm;
    }

    return 0;
}
