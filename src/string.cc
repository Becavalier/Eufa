#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

    void EMSCRIPTEN_KEEPALIVE capitalize (char *string) {
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
    }

#ifdef __cplusplus
}
#endif
