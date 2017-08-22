#ifndef EUFA_GLOBAL
#define EUFA_GLOBAL

#include <inttypes.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <cstdlib>
#include <cmath>
#include <emscripten/emscripten.h>

#define TRUE 1
#define FALSE 0
#define EUFA_CACHE_MAX_MEMORY_SIZE 16777216
#define PREFIX_SIZE sizeof(size_t)
#define increment_cache_used_memory(_n) { \
    all_cache_used_memory += _n; \
}

#define decrement_cache_used_memory(_n) { \
    all_cache_used_memory -= _n; \
}

static size_t all_cache_used_memory = 0;

extern "C" {

    extern void print_i32 (int number);
    extern void print_i8_string (char* offset, int length);
    extern void print_i32_array (int* offset, int length);

    /* Environment detect functions */
    extern size_t sizeof_type_double();
    extern size_t sizeof_type_long();

    /* Memory related functions for cache */
    extern size_t cache_used_memory (void);
    extern void* cache_malloc (size_t size);
    extern void cache_free (void *ptr);

}

#endif
