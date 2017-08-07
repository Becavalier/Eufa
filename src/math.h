#ifndef EUFA_MATH
#define EUFA_MATH

#include<emscripten/emscripten.h>
#include<cstdlib>
#include<cmath>

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

    extern unsigned int i64_abs (int number);
    extern double f64_abs (double number);

    extern unsigned int i64_sqrt (unsigned int number);
    extern double f64_sqrt (double number);
}


#endif