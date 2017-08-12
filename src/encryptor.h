#ifndef EUFA_ENCRYPTOR
#define EUFA_ENCRYPTOR

#include "global.h"
#include <stdlib.h>
#include <emscripten/emscripten.h>

extern "C" {

    extern unsigned char* base64_encode (const unsigned char *str, size_t length);
    extern unsigned char* base64_decode_ex (const unsigned char *str, size_t length);

}


#endif
