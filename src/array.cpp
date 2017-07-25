#include "array.h"

#ifdef __cplusplus
extern "C" {
#endif

    // Expose to js side
    extern int

    int EMSCRIPTEN_KEEPALIVE add (int x, int y) {
        return x + y;
    }


#ifdef __cplusplus
}
#endif
