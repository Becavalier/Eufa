#include "cache.h"

#ifdef __cplusplus
extern "C" {
#endif

    void EMSCRIPTEN_KEEPALIVE cache_clear (void) {
        cacheTypeNode *thisNode = tailTypeNode;
        cacheTypeNode *prevNode = tailTypeNode->prev;

        do {
            prevNode->next = NULL;
            free(thisNode);
            thisNode = prevNode;
            prevNode = prevNode->prev;
        } while(prevNode->prev != NULL);

        // Reset pointers
        tailNode = headNode;
        tailTypeNode = headTypeNode;
    }

#ifdef __cplusplus
}
#endif
