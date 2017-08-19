#include "tensorflow.h"

#ifdef __cplusplus
extern "C" {
#endif

    const char* EMSCRIPTEN_KEEPALIVE tf_version () {
        return TF_Version();
    }

#ifdef __cplusplus
}
#endif
