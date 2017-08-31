#include "global.cc"
#include "math/math.cc"
#include "string/string.cc"
#include "encryptor/md5.cc"
#include "encryptor/sha1.cc"
#include "encryptor/base64.cc"
#include "array/sort.cc"
// #include "tensorflow/tensorflow.cc"
#include "cache/set.cc"
#include "cache/get.cc"
#include "cache/del.cc"
#include "cache/clear.cc"
#include "file/file.cc"
#include "dlib/kmeans.cc"
#include "dlib/dnn.cc"

int main() {
    _INIT_EUFA_CACHE_RUNTIME();
    _INIT_EUFA_FILE_RUNTIME();

    return 1;
}
