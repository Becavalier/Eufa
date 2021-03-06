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
        return abs(number);
    }

    unsigned int EMSCRIPTEN_KEEPALIVE i64_sqrt (unsigned int number) {
        return sqrt(number);
    }

    double EMSCRIPTEN_KEEPALIVE f64_sqrt (double number) {
        return sqrtf(number);
    }

#ifdef __cplusplus
}
#endif
