#include <iostream>
#include <dlfcn.h>
#include <string>

typedef int (*PrimeFunc)(int, int);
typedef float (*SquareFunc)(float, float);

int main() {
    // Загрузка библиотек
    void* primeLib = dlopen("./libPrimeLibrary.so", RTLD_LAZY);
    void* squareLib = dlopen("./libSquareLibrary.so", RTLD_LAZY);

    if (!primeLib || !squareLib) {
        std::cerr << "Failed to load libraries!\n";
        return 1;
    }

    // Загрузка функций
    PrimeFunc PrimeCountNaive = (PrimeFunc)dlsym(primeLib, "PrimeCountNaive");
    PrimeFunc PrimeCountSieve = (PrimeFunc)dlsym(primeLib, "PrimeCountSieve");
    SquareFunc SquareRectangle = (SquareFunc)dlsym(squareLib, "SquareRectangle");
    SquareFunc SquareTriangle = (SquareFunc)dlsym(squareLib, "SquareTriangle");

    if (!PrimeCountNaive || !PrimeCountSieve || !SquareRectangle || !SquareTriangle) {
        std::cerr << "Failed to load functions!\n";
        dlclose(primeLib);
        dlclose(squareLib);
        return 1;
    }

    PrimeFunc currentPrimeFunc = PrimeCountNaive; 
    SquareFunc currentSquareFunc = SquareRectangle; 

    std::cout << "Enter command: \n";
    std::cout << "0 - Switch implementation \n";
    std::cout << "1 arg1 arg2 - Call PrimeCount \n";
    std::cout << "2 arg1 arg2 - Call Square \n";
    std::cout << "Other - Exit \n";

    int command;
    while (std::cin >> command) {
        if (command == 0) {
            if (currentPrimeFunc == PrimeCountNaive) {
                currentPrimeFunc = PrimeCountSieve;
                std::cout << "Switched to Sieve for PrimeCount \n";
            } else {
                currentPrimeFunc = PrimeCountNaive;
                std::cout << "Switched to Naive for PrimeCount \n";
            }

            if (currentSquareFunc == SquareRectangle) {
                currentSquareFunc = SquareTriangle;
                std::cout << "Switched to Triangle for Square \n";
            } else {
                currentSquareFunc = SquareRectangle;
                std::cout << "Switched to Rectangle for Square \n";
            }
        } else if (command == 1) {
            int A, B;
            std::cin >> A >> B;
            std::cout << "PrimeCount result: " << currentPrimeFunc(A, B) << "\n";
        } else if (command == 2) {
            float A, B;
            std::cin >> A >> B;
            std::cout << "Square result: " << currentSquareFunc(A, B) << "\n";
        } else {
            break;
        }
        std::cout << "Enter command: \n";
    }
    dlclose(primeLib);
    dlclose(squareLib);
    return 0;
}
