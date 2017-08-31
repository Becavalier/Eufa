#ifndef EUFA_FILE
#define EUFA_FILE

#include "../global.h"

// Downloading queue.
typedef map<string, int> DOWNLOADING_ENTITY;
typedef stack<DOWNLOADING_ENTITY> DOWNLOADING_STACK;

extern "C" {

    // Init function
    void _INIT_EUFA_FILE_RUNTIME() {
        DOWNLOADING_STACK stack;
    }

    extern void fetchFile(const char*, const char*);
    extern void fetchFileBatch(const char*, em_str_callback_func, em_str_callback_func);

}

#endif
