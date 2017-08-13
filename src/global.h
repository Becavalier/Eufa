#ifndef EUFA_GLOBAL
#define EUFA_GLOBAL

#include <inttypes.h>
#include <emscripten/emscripten.h>

extern "C" {

    extern void print_i32 (int number);
    extern void print_i8_string (char* offset, int length);
    extern void print_i32_array (int* offset, int length);

}

#endif
