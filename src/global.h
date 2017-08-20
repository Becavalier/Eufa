#ifndef EUFA_GLOBAL
#define EUFA_GLOBAL

#include <inttypes.h>
#include <emscripten/emscripten.h>

#define TRUE 1
#define FALSE 0

extern "C" {

    extern void print_i32 (int number);
    extern void print_i8_string (char* offset, int length);
    extern void print_i32_array (int* offset, int length);

    /* Environment detect functions */
    extern size_t EMSCRIPTEN_KEEPALIVE sizeof_type_double () {
        return sizeof(double);
    }

}

#endif
