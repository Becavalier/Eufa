#ifndef EUFA_MATH
#define EUFA_MATH

#include<emscripten/emscripten.h>

using namespace std;

extern "C" {

    extern int i64_add (int x, int y);
    extern double f64_add (double x, double y);

    extern int i64_minus (int x, int y);
    extern double f64_minus (double x, double y);

    extern int i64_multiply (int x, int y);
    extern double f64_multiply (double x, double y);

    extern int i64_divide (int x, int y);
    extern double f64_divide (double x, double y);

}


#endif