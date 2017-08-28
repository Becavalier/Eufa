#ifndef EUFA_DLIB_CNN
#define EUFA_DLIB_CNN

#include "dlib.h"
#include <dlib/dnn.h>
#include <dlib/data_io.h>

// #define MNIST_DEBUG

using namespace std;
using namespace dlib;

extern "C" {

    #ifdef MNIST_DEBUG

    extern const char* testcase_cnn_mnist(unsigned char[]);

    #else

    extern unsigned long testcase_cnn_mnist(unsigned char[]);

    #endif
}

#endif
