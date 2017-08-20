#include "cache.h"

#ifdef __cplusplus
extern "C" {
#endif

    void EMSCRIPTEN_KEEPALIVE del_kv (const char* key) {
        delDataNodeInLinkedList(key);
        delTypeNodeInLinkedList(key);
    }

#ifdef __cplusplus
}
#endif
