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

    int EMSCRIPTEN_KEEPALIVE i64_abs (int x) {
        return abs(x);
    }

    double EMSCRIPTEN_KEEPALIVE f64_abs (double x) {
        return fabs(x);
    }

#ifdef __cplusplus
}
#endif
