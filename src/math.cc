#include "math.h"

#ifdef __cplusplus
extern "C" {
#endif

    int EMSCRIPTEN_KEEPALIVE i64_add (int x, int y) {
        return x + y;
    }

    double EMSCRIPTEN_KEEPALIVE f64_add (double x, double y) {
        return x + y;
    }

    int EMSCRIPTEN_KEEPALIVE i64_minus (int x, int y) {
        return x - y;
    }

    double EMSCRIPTEN_KEEPALIVE f64_minus (double x, double y) {
        return x - y;
    }

    int EMSCRIPTEN_KEEPALIVE i64_multiply (int x, int y) {
        return x * y;
    }

    double EMSCRIPTEN_KEEPALIVE f64_multiply (double x, double y) {
        return x * y;
    }

    int EMSCRIPTEN_KEEPALIVE i64_divide (int x, int y) {
        return x / y;
    }

    double EMSCRIPTEN_KEEPALIVE f64_divide (double x, double y) {
        return x / y;
    }

    unsigned int EMSCRIPTEN_KEEPALIVE i64_abs (int number) {
        return abs(number);
    }

    double EMSCRIPTEN_KEEPALIVE f64_abs (double number) {
        return fabs(number);
    }

    unsigned int EMSCRIPTEN_KEEPALIVE i64_sqrt (unsigned int number) {
        float f;
        unsigned int u;

        f = number;
        u = *(unsigned int *)&f;
        u = (u + 0x3f7a63d7) >> 1;
        f = *(float *) &u;
        u = f;
        u = (u + number/u) >> 1;

        return u;
    }

    double EMSCRIPTEN_KEEPALIVE f64_sqrt (double number) {
        long i;
        float x, y;
        const float f = 1.5F;
        x = number * 0.5F;
        y = number;
        i = *(long *) &y;
        i = 0x5f3759df - (i >> 1);
        y = *(float *) &i;
        y = y * (f - (x * y * y));

        return number * y;
    }

#ifdef __cplusplus
}
#endif
