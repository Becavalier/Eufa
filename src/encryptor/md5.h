#ifndef EUFA_ENCRYPTOR_MD5
#define EUFA_ENCRYPTOR_MD5

#include "encryptor.h"

#define ENCRYPTOR_F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define ENCRYPTOR_G(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define ENCRYPTOR_H(x, y, z) ((x) ^ (y) ^ (z))
#define ENCRYPTOR_I(x, y, z) ((y) ^ ((x) | ~(z)))

#define SET(n) \
    (ctx->block[(n)] = \
	(uint32_t)ptr[(n) * 4] | \
	((uint32_t)ptr[(n) * 4 + 1] << 8) | \
	((uint32_t)ptr[(n) * 4 + 2] << 16) | \
	((uint32_t)ptr[(n) * 4 + 3] << 24))
#define GET(n) \
	(ctx->block[(n)])

/*
 * The MD5 transformation for all four rounds.
 */
#define STEP(f, a, b, c, d, x, t, s) \
    (a) += f((b), (c), (d)) + (x) + (t); \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
    (a) += (b);



extern "C" {

    /*
     *  Expose functions.
     */
    extern char* md5 (const unsigned char *str, size_t length);

    typedef struct {
    	uint32_t lo, hi;
    	uint32_t a, b, c, d;
    	unsigned char buffer[64];
    	uint32_t block[16];
    } MD5_CTX;

    void MD5Init (MD5_CTX *ctx);
    void MD5Update (MD5_CTX *ctx, const unsigned char *data, size_t size);
    void MD5Final (unsigned char *result, MD5_CTX *ctx);

    void make_digest_ex (char *md5str, const unsigned char *digest, int len);

}

#endif
