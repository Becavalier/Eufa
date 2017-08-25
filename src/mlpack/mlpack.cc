#include "core/util/version.hpp"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

    const char* EMSCRIPTEN_KEEPALIVE mlpack_version (void) {
        // return mlpack::util::GetVersion().c_str();
        return string("MLPACK_VERSION").c_str();
    }

#ifdef __cplusplus
}
#endif
