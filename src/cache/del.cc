#include "cache.h"

#ifdef __cplusplus
extern "C" {
#endif

    void EMSCRIPTEN_KEEPALIVE cache_del_kv (const char* key) {
        delDataNodeInLinkedList(key);
        delTypeNodeInLinkedList(key);
    }

#ifdef __cplusplus
}
#endif
