#include "cache.h"

#ifdef __cplusplus
extern "C" {
#endif

    const char* EMSCRIPTEN_KEEPALIVE cache_get_kv_str (const char* key) {
        cacheDataNode* dataNode = searchDataNodeInLinkedList(key);
        if (dataNode != NULL) {
            return dataNode->data->strData;
        } else {
            return NULL;
        }
    }

    double EMSCRIPTEN_KEEPALIVE cache_get_kv_num (const char* key) {
        cacheDataNode* dataNode = searchDataNodeInLinkedList(key);
        if (dataNode != NULL) {
            return dataNode->data->numData;
        } else {
            return FALSE;
        }
    }

#ifdef __cplusplus
}
#endif
