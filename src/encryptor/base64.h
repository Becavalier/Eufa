#ifndef EUFA_ENCRYPTOR_BASE64
#define EUFA_ENCRYPTOR_BASE64

#include "encryptor.h"

extern "C" {

    /*
     *  Expose functions.
     */
    extern unsigned char* base64_encode (const unsigned char *str, size_t length);
    extern unsigned char* base64_decode_ex (const unsigned char *str, size_t length);

}

#endif
