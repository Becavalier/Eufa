#include "encryptor.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* {{{ base64 related */
    static const char base64_table[] = {
    	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
    };

    static const char base64_pad = '=';

    static const short base64_reverse_table[256] = {
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    	-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
    	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
    	-2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
    	-2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
    };

    unsigned char* EMSCRIPTEN_KEEPALIVE base64_encode (const unsigned char *str, size_t length) {
    	const unsigned char *current = str;
    	unsigned char *p;
    	unsigned char *result;

    	result = (unsigned char*) malloc(((length + 2) / 3) * 4 * sizeof(char));
    	p = result;

    	while (length > 2) { /* keep going until we have less than 24 bits */
    		*p++ = base64_table[current[0] >> 2];
    		*p++ = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
    		*p++ = base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
    		*p++ = base64_table[current[2] & 0x3f];

    		current += 3;
    		length -= 3; /* we just handle 3 octets of data */
    	}

    	/* now deal with the tail end of things */
    	if (length != 0) {
    		*p++ = base64_table[current[0] >> 2];
    		if (length > 1) {
    			*p++ = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
    			*p++ = base64_table[(current[1] & 0x0f) << 2];
    			*p++ = base64_pad;
    		} else {
    			*p++ = base64_table[(current[0] & 0x03) << 4];
    			*p++ = base64_pad;
    			*p++ = base64_pad;
    		}
    	}
    	*p = '\0';

    	return result;
    }

    unsigned char* EMSCRIPTEN_KEEPALIVE base64_decode_ex (const unsigned char *str, size_t length) {
    	const unsigned char *current = str;
    	int ch, i = 0, j = 0, padding = 0;
    	unsigned char *result;

    	result = (unsigned char*) malloc(length);

    	/* run through the whole string, converting as we go */
    	while (length-- > 0) {
    		ch = *current++;
    		if (ch == base64_pad) {
    			padding++;
    			continue;
    		}

    		ch = base64_reverse_table[ch];
            if (ch < 0) {
                continue;
            }

    		switch(i % 4) {
    		case 0:
    			result[j] = ch << 2;
    			break;
    		case 1:
    			result[j++] |= ch >> 4;
    			result[j] = (ch & 0x0f) << 4;
    			break;
    		case 2:
    			result[j++] |= ch >>2;
    			result[j] = (ch & 0x03) << 6;
    			break;
    		case 3:
    			result[j++] |= ch;
    			break;
    		}
    		i++;
    	}

    	result[length] = '\0';

    	return result;
    }
    /* }}} */



    /* {{{ md5 related */
    char* EMSCRIPTEN_KEEPALIVE md5 (const unsigned char *str, size_t length) {
    	unsigned char *arg;
    	unsigned char raw_output = 0;
    	char* md5str = (char*) malloc(33 * sizeof(char));
    	MD5_CTX context;
    	unsigned char digest[16];

    	md5str[0] = '\0';
    	MD5Init(&context);
    	MD5Update(&context, str, length);
    	MD5Final(digest, &context);

        make_digest_ex(md5str, digest, 16);
        return md5str;
    }

    void make_digest_ex (char *md5str, const unsigned char *digest, int len) {
    	static const char hexits[17] = "0123456789abcdef";
    	int i;

    	for (i = 0; i < len; i++) {
    		md5str[i * 2] = hexits[digest[i] >> 4];
    		md5str[(i * 2) + 1] = hexits[digest[i] &  0x0F];
    	}
    	md5str[len * 2] = '\0';
    }

    static const unsigned char *md5_body (MD5_CTX *ctx, const unsigned char *data, size_t size) {
    	const unsigned char *ptr;
    	uint32_t a, b, c, d;
    	uint32_t saved_a, saved_b, saved_c, saved_d;

    	ptr = data;

    	a = ctx->a;
    	b = ctx->b;
    	c = ctx->c;
    	d = ctx->d;

    	do {
    		saved_a = a;
    		saved_b = b;
    		saved_c = c;
    		saved_d = d;

            /* Round 1 */
    		STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7)
    		STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12)
    		STEP(F, c, d, a, b, SET(2), 0x242070db, 17)
    		STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22)
    		STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7)
    		STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12)
    		STEP(F, c, d, a, b, SET(6), 0xa8304613, 17)
    		STEP(F, b, c, d, a, SET(7), 0xfd469501, 22)
    		STEP(F, a, b, c, d, SET(8), 0x698098d8, 7)
    		STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12)
    		STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17)
    		STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22)
    		STEP(F, a, b, c, d, SET(12), 0x6b901122, 7)
    		STEP(F, d, a, b, c, SET(13), 0xfd987193, 12)
    		STEP(F, c, d, a, b, SET(14), 0xa679438e, 17)
    		STEP(F, b, c, d, a, SET(15), 0x49b40821, 22)

            /* Round 2 */
    		STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)
    		STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)
    		STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
    		STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
    		STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)
    		STEP(G, d, a, b, c, GET(10), 0x02441453, 9)
    		STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
    		STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
    		STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
    		STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)
    		STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
    		STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)
    		STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
    		STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
    		STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)
    		STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

            /* Round 3 */
    		STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)
    		STEP(H, d, a, b, c, GET(8), 0x8771f681, 11)
    		STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
    		STEP(H, b, c, d, a, GET(14), 0xfde5380c, 23)
    		STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)
    		STEP(H, d, a, b, c, GET(4), 0x4bdecfa9, 11)
    		STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
    		STEP(H, b, c, d, a, GET(10), 0xbebfbc70, 23)
    		STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
    		STEP(H, d, a, b, c, GET(0), 0xeaa127fa, 11)
    		STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
    		STEP(H, b, c, d, a, GET(6), 0x04881d05, 23)
    		STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
    		STEP(H, d, a, b, c, GET(12), 0xe6db99e5, 11)
    		STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
    		STEP(H, b, c, d, a, GET(2), 0xc4ac5665, 23)

            /* Round 4 */
    		STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)
    		STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)
    		STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
    		STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)
    		STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)
    		STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
    		STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
    		STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)
    		STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
    		STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
    		STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)
    		STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
    		STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)
    		STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
    		STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
    		STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)

    		a += saved_a;
    		b += saved_b;
    		c += saved_c;
    		d += saved_d;

    		ptr += 64;
    	} while (size -= 64);

    	ctx->a = a;
    	ctx->b = b;
    	ctx->c = c;
    	ctx->d = d;

    	return ptr;
    }

    void MD5Init (MD5_CTX *ctx) {
    	ctx->a = 0x67452301;
    	ctx->b = 0xefcdab89;
    	ctx->c = 0x98badcfe;
    	ctx->d = 0x10325476;

    	ctx->lo = 0;
    	ctx->hi = 0;
    }

    void MD5Update (MD5_CTX *ctx, const unsigned char *data, size_t size) {
    	uint32_t saved_lo;
    	uint32_t used, free;

    	saved_lo = ctx->lo;
    	if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo) {
    		ctx->hi++;
    	}
    	ctx->hi += size >> 29;

    	used = saved_lo & 0x3f;

    	if (used) {
    		free = 64 - used;

    		if (size < free) {
    			memcpy(&ctx->buffer[used], data, size);
    			return;
    		}

    		memcpy(&ctx->buffer[used], data, free);
    		data = (unsigned char *)data + free;
    		size -= free;
    		md5_body(ctx, ctx->buffer, 64);
    	}

    	if (size >= 64) {
    		data = md5_body(ctx, data, size & ~(size_t)0x3f);
    		size &= 0x3f;
    	}

    	memcpy(ctx->buffer, data, size);
    }

    void MD5Final (unsigned char *result, MD5_CTX *ctx) {
    	uint32_t used, free;

    	used = ctx->lo & 0x3f;

    	ctx->buffer[used++] = 0x80;

    	free = 64 - used;

    	if (free < 8) {
    		memset(&ctx->buffer[used], 0, free);
    		md5_body(ctx, ctx->buffer, 64);
    		used = 0;
    		free = 64;
    	}

    	memset(&ctx->buffer[used], 0, free - 8);

    	ctx->lo <<= 3;
    	ctx->buffer[56] = ctx->lo;
    	ctx->buffer[57] = ctx->lo >> 8;
    	ctx->buffer[58] = ctx->lo >> 16;
    	ctx->buffer[59] = ctx->lo >> 24;
    	ctx->buffer[60] = ctx->hi;
    	ctx->buffer[61] = ctx->hi >> 8;
    	ctx->buffer[62] = ctx->hi >> 16;
    	ctx->buffer[63] = ctx->hi >> 24;

    	md5_body(ctx, ctx->buffer, 64);

    	result[0] = ctx->a;
    	result[1] = ctx->a >> 8;
    	result[2] = ctx->a >> 16;
    	result[3] = ctx->a >> 24;
    	result[4] = ctx->b;
    	result[5] = ctx->b >> 8;
    	result[6] = ctx->b >> 16;
    	result[7] = ctx->b >> 24;
    	result[8] = ctx->c;
    	result[9] = ctx->c >> 8;
    	result[10] = ctx->c >> 16;
    	result[11] = ctx->c >> 24;
    	result[12] = ctx->d;
    	result[13] = ctx->d >> 8;
    	result[14] = ctx->d >> 16;
    	result[15] = ctx->d >> 24;

    	EUFA_ZERO(ctx, sizeof(*ctx));
    }
    /* }}} */


#ifdef __cplusplus
}
#endif
