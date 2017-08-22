#include "cache.h"

#ifdef __cplusplus
extern "C" {
#endif

    void EMSCRIPTEN_KEEPALIVE cache_set_kv_str (const char* key, const char* value) {
        cacheDataObject *data = (cacheDataObject*) cache_malloc(sizeof(cacheDataObject));
        cacheDataNode *dataNode = (cacheDataNode*) cache_malloc(sizeof(cacheDataNode));
        // Fill up
        data->strData  = value;
        dataNode->key  = key;
        dataNode->data = data;
        // Immutable
        delDataNodeInLinkedList(key);
        // Add
        addDataNodeToLinkedList(dataNode);
    }

    void EMSCRIPTEN_KEEPALIVE cache_set_kv_num (const char* key, double value) {
        cacheDataObject *data = (cacheDataObject*) cache_malloc(sizeof(cacheDataObject));
        cacheDataNode *dataNode = (cacheDataNode*) cache_malloc(sizeof(cacheDataNode));
        // Fill up
        data->numData  = value;
        dataNode->key  = key;
        dataNode->data = data;
        // Immutable
        delDataNodeInLinkedList(key);
        // Add
        addDataNodeToLinkedList(dataNode);
    }

    void EMSCRIPTEN_KEEPALIVE cache_set_type (const char* key, const unsigned char type) {
        cacheTypeNode *typeNode = (cacheTypeNode*) cache_malloc(sizeof(cacheTypeNode));
        // Fill up
        typeNode->symbol = key;
        typeNode->type   = type;
        // Immutable
        delTypeNodeInLinkedList(key);
        // Add
        addTypeNodeToLinkedList(typeNode);
    }

#ifdef __cplusplus
}
#endif
