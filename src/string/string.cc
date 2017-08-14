#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

    unsigned char* EMSCRIPTEN_KEEPALIVE capitalize (unsigned char *string) {
        int i = 0;
        while (1) {
            char _current_pos = *(string + i);

            if (_current_pos >= 0x61 && _current_pos <= 0x7a) {
                *(string + i) = _current_pos - 32;
            }

            if (_current_pos == '\0') {
                break;
            }

            i++;
        }

        return string;
    }

#ifdef __cplusplus
}
#endif
