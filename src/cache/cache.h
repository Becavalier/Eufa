#ifndef EUFA_CACHE
#define EUFA_CACHE

#include "../global.h"
#include "string.h"

#define EUFA_CACHE_HEAD_LIST_KEY "EUFA_CACHE_HEAD"
#define EUFA_CACHE_HEAD_LIST_VAL "EUFA_CACHE_HEAD_DATA"
#define EUFA_CACHE_TYPE_NUM 1
#define EUFA_CACHE_TYPE_STR 2


extern "C" {

    typedef union cacheDataObject {
        const char* strData;
        double numData;
    } cacheDataObject;

    typedef struct cacheDataNode {
        const char* key;
        cacheDataObject *data;
        struct cacheDataNode *next;
        struct cacheDataNode *prev;
    } cacheDataNode;

    typedef struct cacheTypeNode {
        const char* symbol;
        /**
            1 - number (int, double)
            2 - string like (char*)
        */
        unsigned char type;
        struct cacheTypeNode *next;
        struct cacheTypeNode *prev;
    } cacheTypeNode;


    // Global doubly linked list
    const char* headNodeKey = EUFA_CACHE_HEAD_LIST_KEY;
    const char* headNodeVal = EUFA_CACHE_HEAD_LIST_VAL;

    cacheDataObject *headDataObject = (cacheDataObject*) cache_malloc(sizeof(cacheDataObject));

    cacheDataNode *headNode = (cacheDataNode*) cache_malloc(sizeof(cacheDataNode));
    cacheDataNode *tailNode = headNode;

    cacheTypeNode *headTypeNode = (cacheTypeNode*) cache_malloc(sizeof(cacheTypeNode));
    cacheTypeNode *tailTypeNode = headTypeNode;

    // Init function
    void _INIT_EUFA_CACHE_RUNTIME() {
        headDataObject->strData = headNodeVal;
        headNode->key           = headNodeKey;
        headNode->data          = headDataObject;
        headNode->next          = NULL;
        headNode->prev          = NULL;
        headTypeNode->symbol    = headNodeKey;
        headTypeNode->type      = EUFA_CACHE_TYPE_STR;
        headTypeNode->next      = NULL;
        headTypeNode->prev      = NULL;
    }

    // Global methods
    void addDataNodeToLinkedList (cacheDataNode* node) {
        node->prev     = tailNode;
        node->next     = NULL;
        tailNode->next = node;
        tailNode       = node;
    }

    cacheDataNode* searchDataNodeInLinkedList (const char* key) {
        cacheDataNode *thisNode = tailNode;
        do {
            if (strcmp(key, thisNode->key) == 0) {
                return thisNode;
            }
            thisNode = thisNode->prev;
        } while(thisNode != NULL);
        return NULL;
    }

    void delDataNodeInLinkedList (const char* key) {
        cacheDataNode *thisNode = tailNode;
        do {
            if (strcmp(key, thisNode->key) == 0) {
                if (thisNode->prev != NULL && thisNode->next != NULL) {
                    thisNode->prev->next = thisNode->next;
                    thisNode->next->prev = thisNode->prev;
                } else if (thisNode->prev != NULL) {
                    thisNode->prev->next = NULL;
                } else if (thisNode->next != NULL) {
                    thisNode->next->prev = NULL;
                }

                if (thisNode == tailNode) {
                    tailNode = thisNode->prev;
                }

                cache_free(thisNode->data);
                cache_free(thisNode);
                return;
            }
            thisNode = thisNode->prev;
        } while(thisNode != NULL);
    }


    // Type table related
    void addTypeNodeToLinkedList (cacheTypeNode* node) {
        node->prev         = tailTypeNode;
        node->next         = NULL;
        tailTypeNode->next = node;
        tailTypeNode       = node;
    }
        
    unsigned char EMSCRIPTEN_KEEPALIVE searchTypeNode (const char* key) {
        cacheTypeNode *thisNode = tailTypeNode;
        do {
            if (strcmp(key, thisNode->symbol) == 0) {
                return thisNode->type;
            }
            thisNode = thisNode->prev;
        } while(thisNode != NULL);
        return FALSE;
    }

    void delTypeNodeInLinkedList (const char* key) {
        cacheTypeNode *thisNode = tailTypeNode;
        do {
            if (strcmp(key, thisNode->symbol) == 0) {
                if (thisNode->prev != NULL && thisNode->next != NULL) {
                    thisNode->prev->next = thisNode->next;
                    thisNode->next->prev = thisNode->prev;
                } else if (thisNode->prev != NULL) {
                    thisNode->prev->next = NULL;
                } else if (thisNode->next != NULL) {
                    thisNode->next->prev = NULL;
                }

                if (thisNode == tailTypeNode) {
                    tailTypeNode = thisNode->prev;
                }

                cache_free(thisNode);
                return;
            }
            thisNode = thisNode->prev;
        } while(thisNode != NULL);
    }

    // Export functions
    extern void cache_set_type (const char* key, const unsigned char type);
    extern void cache_set_kv_str (const char* key, const char* value);
    extern void cache_set_kv_num (const char* key, double value);

    extern const char* cache_get_kv_str (const char* key);
    extern const double cache_get_kv_num (const char* key);

    extern void cache_del_kv (const char* key);

    extern void cache_clear (void);
}


#endif
