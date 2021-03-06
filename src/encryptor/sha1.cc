#include "sha1.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* {{{ sha1 related */
    char* EMSCRIPTEN_KEEPALIVE sha1 (const unsigned char *str, size_t length) {
    	unsigned char *arg;
    	unsigned char raw_output = 0;
    	char* sha1str = (char*) malloc(41 * sizeof(char));
    	SHA1_CTX context;
    	unsigned char digest[20];

    	sha1str[0] = '\0';
    	SHA1Init(&context);
    	SHA1Update(&context, str, length);
    	SHA1Final(digest, &context);

    	make_digest_ex(sha1str, digest, 20);
        return sha1str;
    }

    void SHA1Init(SHA1_CTX * context) {
    	context->count[0] = context->count[1] = 0;
    	/* Load magic initialization constants. */
    	context->state[0] = 0x67452301;
    	context->state[1] = 0xefcdab89;
    	context->state[2] = 0x98badcfe;
    	context->state[3] = 0x10325476;
    	context->state[4] = 0xc3d2e1f0;
    }


    /*
       SHA1 block update operation. Continues an SHA1 message-digest
       operation, processing another message block, and updating the
       context.
     */
    void SHA1Update (SHA1_CTX * context, const unsigned char *input, size_t inputLen) {
    	unsigned int i, index, partLen;

    	/* Compute number of bytes mod 64 */
    	index = (unsigned int) ((context->count[0] >> 3) & 0x3F);

    	/* Update number of bits */
    	if ((context->count[0] += ((uint32_t) inputLen << 3))
    		< ((uint32_t) inputLen << 3))
    		context->count[1]++;
    	context->count[1] += ((uint32_t) inputLen >> 29);

    	partLen = 64 - index;

    	/* Transform as many times as possible. */
    	if (inputLen >= partLen) {
    		memcpy
    			((unsigned char*) & context->buffer[index], (unsigned char*) input, partLen);
    		SHA1Transform(context->state, context->buffer);

    		for (i = partLen; i + 63 < inputLen; i += 64)
    			SHA1Transform(context->state, &input[i]);

    		index = 0;
    	} else
    		i = 0;

    	/* Buffer remaining input */
    	memcpy ((unsigned char*) & context->buffer[index], (unsigned char*) & input[i], inputLen - i);
    }


    /*
       SHA1 finalization. Ends an SHA1 message-digest operation, writing the
       the message digest and zeroizing the context.
     */
    void SHA1Final (unsigned char digest[20], SHA1_CTX * context) {
    	unsigned char bits[8];
    	unsigned int index, padLen;

    	/* Save number of bits */
    	bits[7] = context->count[0] & 0xFF;
    	bits[6] = (context->count[0] >> 8) & 0xFF;
    	bits[5] = (context->count[0] >> 16) & 0xFF;
    	bits[4] = (context->count[0] >> 24) & 0xFF;
    	bits[3] = context->count[1] & 0xFF;
    	bits[2] = (context->count[1] >> 8) & 0xFF;
    	bits[1] = (context->count[1] >> 16) & 0xFF;
    	bits[0] = (context->count[1] >> 24) & 0xFF;

    	/* Pad out to 56 mod 64. */
    	index = (unsigned int) ((context->count[0] >> 3) & 0x3f);
    	padLen = (index < 56) ? (56 - index) : (120 - index);
    	SHA1Update(context, PADDING, padLen);

    	/* Append length (before padding) */
    	SHA1Update(context, bits, 8);

    	/* Store state in digest */
    	SHA1Encode(digest, context->state, 20);

    	/* Zeroize sensitive information.
    	 */
    	memset((unsigned char*) context, 0, sizeof(*context));
    }


    /*
     * SHA1 basic transformation. Transforms state based on block.
     */
    static void SHA1Transform (uint32_t state[5], const unsigned char block[64]) {
    	uint32_t a = state[0], b = state[1], c = state[2];
    	uint32_t d = state[3], e = state[4], x[16], tmp;

    	SHA1Decode(x, block, 64);

    	/* Round 1 */
    	SHA1_FF(a, b, c, d, e, x[0]);   /* 1 */
    	SHA1_FF(e, a, b, c, d, x[1]);   /* 2 */
    	SHA1_FF(d, e, a, b, c, x[2]);   /* 3 */
    	SHA1_FF(c, d, e, a, b, x[3]);   /* 4 */
    	SHA1_FF(b, c, d, e, a, x[4]);   /* 5 */
    	SHA1_FF(a, b, c, d, e, x[5]);   /* 6 */
    	SHA1_FF(e, a, b, c, d, x[6]);   /* 7 */
    	SHA1_FF(d, e, a, b, c, x[7]);   /* 8 */
    	SHA1_FF(c, d, e, a, b, x[8]);   /* 9 */
    	SHA1_FF(b, c, d, e, a, x[9]);   /* 10 */
    	SHA1_FF(a, b, c, d, e, x[10]);  /* 11 */
    	SHA1_FF(e, a, b, c, d, x[11]);  /* 12 */
    	SHA1_FF(d, e, a, b, c, x[12]);  /* 13 */
    	SHA1_FF(c, d, e, a, b, x[13]);  /* 14 */
    	SHA1_FF(b, c, d, e, a, x[14]);  /* 15 */
    	SHA1_FF(a, b, c, d, e, x[15]);  /* 16 */
    	SHA1_FF(e, a, b, c, d, SHA1_W(16));  /* 17 */
    	SHA1_FF(d, e, a, b, c, SHA1_W(17));  /* 18 */
    	SHA1_FF(c, d, e, a, b, SHA1_W(18));  /* 19 */
    	SHA1_FF(b, c, d, e, a, SHA1_W(19));  /* 20 */

    	/* Round 2 */
    	SHA1_GG(a, b, c, d, e, SHA1_W(20));  /* 21 */
    	SHA1_GG(e, a, b, c, d, SHA1_W(21));  /* 22 */
    	SHA1_GG(d, e, a, b, c, SHA1_W(22));  /* 23 */
    	SHA1_GG(c, d, e, a, b, SHA1_W(23));  /* 24 */
    	SHA1_GG(b, c, d, e, a, SHA1_W(24));  /* 25 */
    	SHA1_GG(a, b, c, d, e, SHA1_W(25));  /* 26 */
    	SHA1_GG(e, a, b, c, d, SHA1_W(26));  /* 27 */
    	SHA1_GG(d, e, a, b, c, SHA1_W(27));  /* 28 */
    	SHA1_GG(c, d, e, a, b, SHA1_W(28));  /* 29 */
    	SHA1_GG(b, c, d, e, a, SHA1_W(29));  /* 30 */
    	SHA1_GG(a, b, c, d, e, SHA1_W(30));  /* 31 */
    	SHA1_GG(e, a, b, c, d, SHA1_W(31));  /* 32 */
    	SHA1_GG(d, e, a, b, c, SHA1_W(32));  /* 33 */
    	SHA1_GG(c, d, e, a, b, SHA1_W(33));  /* 34 */
    	SHA1_GG(b, c, d, e, a, SHA1_W(34));  /* 35 */
    	SHA1_GG(a, b, c, d, e, SHA1_W(35));  /* 36 */
    	SHA1_GG(e, a, b, c, d, SHA1_W(36));  /* 37 */
    	SHA1_GG(d, e, a, b, c, SHA1_W(37));  /* 38 */
    	SHA1_GG(c, d, e, a, b, SHA1_W(38));  /* 39 */
    	SHA1_GG(b, c, d, e, a, SHA1_W(39));  /* 40 */

    	/* Round 3 */
    	SHA1_HH(a, b, c, d, e, SHA1_W(40));  /* 41 */
    	SHA1_HH(e, a, b, c, d, SHA1_W(41));  /* 42 */
    	SHA1_HH(d, e, a, b, c, SHA1_W(42));  /* 43 */
    	SHA1_HH(c, d, e, a, b, SHA1_W(43));  /* 44 */
    	SHA1_HH(b, c, d, e, a, SHA1_W(44));  /* 45 */
    	SHA1_HH(a, b, c, d, e, SHA1_W(45));  /* 46 */
    	SHA1_HH(e, a, b, c, d, SHA1_W(46));  /* 47 */
    	SHA1_HH(d, e, a, b, c, SHA1_W(47));  /* 48 */
    	SHA1_HH(c, d, e, a, b, SHA1_W(48));  /* 49 */
    	SHA1_HH(b, c, d, e, a, SHA1_W(49));  /* 50 */
    	SHA1_HH(a, b, c, d, e, SHA1_W(50));  /* 51 */
    	SHA1_HH(e, a, b, c, d, SHA1_W(51));  /* 52 */
    	SHA1_HH(d, e, a, b, c, SHA1_W(52));  /* 53 */
    	SHA1_HH(c, d, e, a, b, SHA1_W(53));  /* 54 */
    	SHA1_HH(b, c, d, e, a, SHA1_W(54));  /* 55 */
    	SHA1_HH(a, b, c, d, e, SHA1_W(55));  /* 56 */
    	SHA1_HH(e, a, b, c, d, SHA1_W(56));  /* 57 */
    	SHA1_HH(d, e, a, b, c, SHA1_W(57));  /* 58 */
    	SHA1_HH(c, d, e, a, b, SHA1_W(58));  /* 59 */
    	SHA1_HH(b, c, d, e, a, SHA1_W(59));  /* 60 */

    	/* Round 4 */
    	SHA1_II(a, b, c, d, e, SHA1_W(60));  /* 61 */
    	SHA1_II(e, a, b, c, d, SHA1_W(61));  /* 62 */
    	SHA1_II(d, e, a, b, c, SHA1_W(62));  /* 63 */
    	SHA1_II(c, d, e, a, b, SHA1_W(63));  /* 64 */
    	SHA1_II(b, c, d, e, a, SHA1_W(64));  /* 65 */
    	SHA1_II(a, b, c, d, e, SHA1_W(65));  /* 66 */
    	SHA1_II(e, a, b, c, d, SHA1_W(66));  /* 67 */
    	SHA1_II(d, e, a, b, c, SHA1_W(67));  /* 68 */
    	SHA1_II(c, d, e, a, b, SHA1_W(68));  /* 69 */
    	SHA1_II(b, c, d, e, a, SHA1_W(69));  /* 70 */
    	SHA1_II(a, b, c, d, e, SHA1_W(70));  /* 71 */
    	SHA1_II(e, a, b, c, d, SHA1_W(71));  /* 72 */
    	SHA1_II(d, e, a, b, c, SHA1_W(72));  /* 73 */
    	SHA1_II(c, d, e, a, b, SHA1_W(73));  /* 74 */
    	SHA1_II(b, c, d, e, a, SHA1_W(74));  /* 75 */
    	SHA1_II(a, b, c, d, e, SHA1_W(75));  /* 76 */
    	SHA1_II(e, a, b, c, d, SHA1_W(76));  /* 77 */
    	SHA1_II(d, e, a, b, c, SHA1_W(77));  /* 78 */
    	SHA1_II(c, d, e, a, b, SHA1_W(78));  /* 79 */
    	SHA1_II(b, c, d, e, a, SHA1_W(79));  /* 80 */

    	state[0] += a;
    	state[1] += b;
    	state[2] += c;
    	state[3] += d;
    	state[4] += e;

    	/* Zeroize sensitive information. */
    	memset((unsigned char*) x, 0, sizeof(x));
    }

    /*
       Encodes input (uint32_t) into output (unsigned char). Assumes len is
       a multiple of 4.
     */
    static void SHA1Encode (unsigned char *output, uint32_t *input, unsigned int len) {
    	unsigned int i, j;

    	for (i = 0, j = 0; j < len; i++, j += 4) {
    		output[j] = (unsigned char) ((input[i] >> 24) & 0xff);
    		output[j + 1] = (unsigned char) ((input[i] >> 16) & 0xff);
    		output[j + 2] = (unsigned char) ((input[i] >> 8) & 0xff);
    		output[j + 3] = (unsigned char) (input[i] & 0xff);
    	}
    }

    /*
       Decodes input (unsigned char) into output (uint32_t). Assumes len is
       a multiple of 4.
     */
    static void SHA1Decode (uint32_t *output, const unsigned char *input, unsigned int len){
    	unsigned int i, j;

    	for (i = 0, j = 0; j < len; i++, j += 4)
    		output[i] = ((uint32_t) input[j + 3]) | (((uint32_t) input[j + 2]) << 8) |
    			(((uint32_t) input[j + 1]) << 16) | (((uint32_t) input[j]) << 24);
    }
    /* }}} */


#ifdef __cplusplus
}
#endif
