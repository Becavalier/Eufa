#include "cache.h"

#ifdef __cplusplus
extern "C" {
#endif

    void EMSCRIPTEN_KEEPALIVE set_kv_str (const char* key, const char* value) {
        cacheDataObject *data = (cacheDataObject*) malloc(sizeof(cacheDataObject));
        cacheDataNode *dataNode = (cacheDataNode*) malloc(sizeof(cacheDataNode));
        cacheTypeNode *typeNode = (cacheTypeNode*) malloc(sizeof(cacheTypeNode));

        data->strData = value;
        dataNode->key = key;
        dataNode->data = data;
        typeNode->symbol = key;
        typeNode->type = EUFA_CACHE_TYPE_STR;

        // Immutable
        delDataNodeInLinkedList(key);
        delTypeNodeInLinkedList(key);
        // Add
        addDataNodeToLinkedList(dataNode);
        addTypeNodeToLinkedList(typeNode);
    }

    void EMSCRIPTEN_KEEPALIVE set_kv_num (const char* key, double value) {
        cacheDataObject *data = (cacheDataObject*) malloc(sizeof(cacheDataObject));
        cacheDataNode *dataNode = (cacheDataNode*) malloc(sizeof(cacheDataNode));
        cacheTypeNode *typeNode = (cacheTypeNode*) malloc(sizeof(cacheTypeNode));

        data->numData = value;
        dataNode->key = key;
        dataNode->data = data;
        typeNode->symbol = key;
        typeNode->type = EUFA_CACHE_TYPE_NUM;

        // Immutable
        delDataNodeInLinkedList(key);
        delTypeNodeInLinkedList(key);
        // Add
        addDataNodeToLinkedList(dataNode);
        addTypeNodeToLinkedList(typeNode);
    }

#ifdef __cplusplus
}
#endif
