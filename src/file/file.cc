#include "file.h"


void fetchOnSucceed (const char* fileName) {
    emscripten_log(EM_LOG_CONSOLE, "[Eufa] File '%s' loading succeed!", fileName);
}

void fetchOnFailed (const char* fileName) {
    emscripten_log(EM_LOG_ERROR, "[Eufa] File '%s' loading failed!", fileName);
}

void EMSCRIPTEN_KEEPALIVE fetchFile(const char* fileName, const char* url) {
    emscripten_async_wget(url, fileName, &fetchOnSucceed, &fetchOnFailed);
}

void EMSCRIPTEN_KEEPALIVE fetchFileBatch(const char* jsonFilesData, em_socket_error_callback su, em_socket_error_callback fa) {

}