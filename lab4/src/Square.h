#ifndef SQUARE_H
#define SQUARE_H

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" EXPORT float SquareRectangle(float A, float B);

extern "C" EXPORT float SquareTriangle(float A, float B);

#endif
