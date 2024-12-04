#include <iostream>
#include "PrimeCount.h"
#include "Square.h"

int main() {
    int choice;
    std::cout << "Enter command: \n";
    std::cout << "1 arg1 arg2 - Prime count \n";
    std::cout << "2 arg1 arg2 - Square \n";
    std::cout << "Other - Exit \n";
    while (std::cin >> choice) {
        if (choice == 1) {
            int A, B;
            std::cin >> A >> B;
            std::cout << "Naive Count: " << PrimeCountNaive(A, B) << "\n";
            std::cout << "Sieve Count: " << PrimeCountSieve(A, B) << "\n";
        } else if (choice == 2) {
            float A, B;
            std::cin >> A >> B;
            std::cout << "Rectangle Area: " << SquareRectangle(A, B) << "\n";
            std::cout << "Triangle Area: " << SquareTriangle(A, B) << "\n";
        } else {
            std::cout << "Exit... \n";
            break;
        }
        std::cout << "Enter command: \n";  
    }
    return 0;
}
