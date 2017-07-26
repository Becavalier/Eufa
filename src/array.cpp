#include "array.h"

#ifdef __cplusplus
extern "C" {
#endif

    int EMSCRIPTEN_KEEPALIVE add (int x, int y) {
        return x + y;
    }


    double EMSCRIPTEN_KEEPALIVE add (double x, double y) {
        return x + y;
    }

#ifdef __cplusplus
}
#endif
