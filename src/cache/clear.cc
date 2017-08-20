#include "cache.h"

#ifdef __cplusplus
extern "C" {
#endif

    void EMSCRIPTEN_KEEPALIVE clear_kv (void) {
        cacheTypeNode *thisNode = tailTypeNode;
        cacheTypeNode *prevNode = tailTypeNode->prev;

        do {
            free(thisNode);
            thisNode = prevNode;
            prevNode = prevNode->prev;
        } while(prevNode->prev == NULL);

        // Reset pointers
        tailNode = headNode;
        tailTypeNode = headTypeNode;
    }

#ifdef __cplusplus
}
#endif
