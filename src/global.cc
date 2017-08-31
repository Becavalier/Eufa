#include "global.h"

#ifdef __cplusplus
      extern "C" {
      #endif

    size_t EMSCRIPTEN_KEEPALIVE sizeof_type_double () {
        return sizeof(double);
    }

    size_t EMSCRIPTEN_KEEPALIVE sizeof_type_long () {
        return sizeof(long);
    }

    size_t EMSCRIPTEN_KEEPALIVE sizeof_type_size_t () {
        return sizeof(size_t);
    }

    void* EMSCRIPTEN_KEEPALIVE cache_malloc (size_t size) {
        if (all_cache_used_memory + size > EUFA_CACHE_MAX_MEMORY_SIZE) {
            return NULL;
        }
        void *ptr = malloc(size + PREFIX_SIZE);
        *((size_t*)ptr) = size;
        update_zmalloc_stat_alloc(size + PREFIX_SIZE);
        return (char*)ptr + PREFIX_SIZE;
    }

    void EMSCRIPTEN_KEEPALIVE cache_free (void *ptr) {
        if (ptr == NULL) {
            return;
        }
        void *realptr = (char*)ptr - PREFIX_SIZE;
        size_t oldsize = *((size_t*)realptr);
        update_zmalloc_stat_free(oldsize + PREFIX_SIZE);
        free(realptr);
    }

    size_t EMSCRIPTEN_KEEPALIVE cache_used_memory (void) {
        return all_cache_used_memory;
    }


#ifdef __cplusplus
}
#endif
