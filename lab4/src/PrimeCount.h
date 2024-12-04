#ifndef PRIMECOUNT_H
#define PRIMECOUNT_H

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" EXPORT int PrimeCountNaive(int A, int B);

extern "C" EXPORT int PrimeCountSieve(int A, int B);

#endif
