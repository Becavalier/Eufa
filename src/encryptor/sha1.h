#ifndef EUFA_ENCRYPTOR_SHA1
#define EUFA_ENCRYPTOR_SHA1

#include "encryptor.h"


extern "C" {

    /*
     *  Expose functions.
     */
    char* sha1 (const unsigned char *str, size_t length);

    typedef struct {
    	uint32_t state[5];		    /* state (ABCD) */
    	uint32_t count[2];		    /* number of bits, modulo 2^64 (lsb first) */
    	unsigned char buffer[64];	/* input buffer */
    } SHA1_CTX;

    void SHA1Init(SHA1_CTX *);
    void SHA1Update(SHA1_CTX *, const unsigned char *, size_t);
    void SHA1Final(unsigned char[20], SHA1_CTX *);
    void make_sha1_digest(char *sha1str, unsigned char *digest);

    static void SHA1Transform(uint32_t[5], const unsigned char[64]);
    static void SHA1Encode(unsigned char *, uint32_t *, unsigned int);
    static void SHA1Decode(uint32_t *, const unsigned char *, unsigned int);

    static unsigned char PADDING[64] = {
    	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    /* F, G, H and I are basic SHA1 functions. */
    #define SHA1_F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
    #define SHA1_G(x, y, z) ((x) ^ (y) ^ (z))
    #define SHA1_H(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
    #define SHA1_I(x, y, z) ((x) ^ (y) ^ (z))

    /* ROTATE_LEFT rotates x left n bits. */
    #define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

    /* W[i] */
    #define SHA1_W(i) ( tmp=x[(i-3)&15]^x[(i-8)&15]^x[(i-14)&15]^x[i&15], \
    	(x[i&15]=ROTATE_LEFT(tmp, 1)) )

    /* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4. */
    #define FF(a, b, c, d, e, w) { \
     (e) += SHA1_F ((b), (c), (d)) + (w) + (uint32_t)(0x5A827999); \
     (e) += ROTATE_LEFT ((a), 5); \
     (b) = ROTATE_LEFT((b), 30); \
    }
    #define GG(a, b, c, d, e, w) { \
     (e) += SHA1_G ((b), (c), (d)) + (w) + (uint32_t)(0x6ED9EBA1); \
     (e) += ROTATE_LEFT ((a), 5); \
     (b) = ROTATE_LEFT((b), 30); \
    }
    #define HH(a, b, c, d, e, w) { \
     (e) += SHA1_H ((b), (c), (d)) + (w) + (uint32_t)(0x8F1BBCDC); \
     (e) += ROTATE_LEFT ((a), 5); \
     (b) = ROTATE_LEFT((b), 30); \
    }
    #define II(a, b, c, d, e, w) { \
     (e) += SHA1_I ((b), (c), (d)) + (w) + (uint32_t)(0xCA62C1D6); \
     (e) += ROTATE_LEFT ((a), 5); \
     (b) = ROTATE_LEFT((b), 30); \
    }

}

#endif
