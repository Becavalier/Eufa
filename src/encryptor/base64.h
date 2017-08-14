#ifndef EUFA_ENCRYPTOR_MD5
#define EUFA_ENCRYPTOR_MD5

#include "encryptor.h"

extern "C" {

    /*
     *  Expose functions.
     */
    extern unsigned char* base64_encode (const unsigned char *str, size_t length);
    extern unsigned char* base64_decode_ex (const unsigned char *str, size_t length);

}

#endif
