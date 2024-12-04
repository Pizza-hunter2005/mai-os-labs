#include "PrimeCount.h"
#include <cmath>
#include <vector>

// Наивный алгоритм
int PrimeCountNaive(int A, int B) {
    int count = 0;
    for (int num = A; num <= B; ++num) {
        bool is_prime = (num > 1);
        for (int i = 2; i <= std::sqrt(num); ++i) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) ++count;
    }
    return count;
}

// Решето Эратосфена
int PrimeCountSieve(int A, int B) {
    if (B < 2) return 0;
    std::vector<bool> sieve(B + 1, true);
    sieve[0] = sieve[1] = false;

    for (int i = 2; i * i <= B; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= B; j += i) {
                sieve[j] = false;
            }
        }
    }
    int count = 0;
    for (int i = A; i <= B; ++i) {
        if (sieve[i]) ++count;
    }
    return count;
}
