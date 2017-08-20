#include "cache.h"

#ifdef __cplusplus
extern "C" {
#endif

    const char* EMSCRIPTEN_KEEPALIVE get_kv_str (const char* key) {
        cacheDataNode* dataNode = searchDataNodeInLinkedList(key);
        return dataNode->data->strData;
    }

    double EMSCRIPTEN_KEEPALIVE get_kv_num (const char* key) {
        cacheDataNode* dataNode = searchDataNodeInLinkedList(key);
        return dataNode->data->numData;
    }

#ifdef __cplusplus
}
#endif
