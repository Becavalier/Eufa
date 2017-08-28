#include "dnn.h"

#define MNIST_PIXEL_WIDTH 28
#define MNIST_PIXEL_HEIGHT 28
#define MNIST_BATCH_COUNT 1


// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
    This is an example illustrating the use of the deep learning tools from the
    dlib C++ Library.  In it, we will train the venerable LeNet convolutional
    neural network to recognize hand written digits.  The network will take as
    input a small image and classify it as one of the 10 numeric digits between
    0 and 9.
    The specific network we will run is from the paper
        LeCun, Yann, et al. "Gradient-based learning applied to document recognition."
        Proceedings of the IEEE 86.11 (1998): 2278-2324.
    except that we replace the sigmoid non-linearities with rectified linear units.
    These tools will use CUDA and cuDNN to drastically accelerate network
    training and testing.  CMake should automatically find them if they are
    installed and configure things appropriately.  If not, the program will
    still run but will be much slower to execute.
*/

#ifdef MNIST_DEBUG

const char* EMSCRIPTEN_KEEPALIVE testcase_cnn_mnist(unsigned char pixel[]) {

#else

unsigned long EMSCRIPTEN_KEEPALIVE testcase_cnn_mnist(unsigned char pixel[]) {

#endif

    // MNIST is broken into two parts, a training set of 60000 images and a test set of
    // 10000 images.  Each image is labeled so that we know what hand written digit is
    // depicted.  These next statements load the dataset into memory.
    std::vector<matrix<rgb_pixel>> test_images_rgb;
    std::vector<matrix<unsigned char>> test_images;

    test_images.resize(MNIST_BATCH_COUNT);
    test_images_rgb.resize(MNIST_BATCH_COUNT);

    for (size_t k = 0; k < MNIST_BATCH_COUNT; k++) {
        test_images[k].set_size(MNIST_PIXEL_WIDTH, MNIST_PIXEL_HEIGHT);
        test_images_rgb[k].set_size(MNIST_PIXEL_WIDTH, MNIST_PIXEL_HEIGHT);

        size_t counter = 0;
        for (int i = 0; i < MNIST_PIXEL_WIDTH; i++) {
            for (int j = 0; j < MNIST_PIXEL_HEIGHT; j++) {
                test_images_rgb[k](i, j).red = pixel[counter++];
                test_images_rgb[k](i, j).green = pixel[counter++];
                test_images_rgb[k](i, j).blue = pixel[counter++];
                counter+=1;
            }
        }
        assign_image(test_images[k], test_images_rgb[k]);
    }



    #ifdef MNIST_DEBUG

    // stringify to json for browser.
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartArray();
    writer.StartArray();
    for (size_t k = 0; k < MNIST_BATCH_COUNT; k++) {
        for (int i = 0; i < MNIST_PIXEL_WIDTH; i++) {
            for (int j = 0; j < MNIST_PIXEL_HEIGHT; j++) {
                writer.Uint(test_images[k](i, j));
            }
        }
    }
    writer.EndArray();
    writer.StartArray();
    for (size_t k = 0; k < MNIST_BATCH_COUNT; k++) {
        for (int i = 0; i < MNIST_PIXEL_WIDTH; i++) {
            for (int j = 0; j < MNIST_PIXEL_HEIGHT; j++) {
                writer.Uint(test_images_rgb[k](i, j).red);
                writer.Uint(test_images_rgb[k](i, j).green);
                writer.Uint(test_images_rgb[k](i, j).blue);
            }
        }
    }
    writer.EndArray();
    writer.StartArray();
        for (int i = 0; i < 4 * MNIST_PIXEL_WIDTH * MNIST_PIXEL_HEIGHT; i++) {
            writer.Uint(pixel[i]);
        }
    writer.EndArray();
    writer.EndArray();
    return s.GetString();

    #else

    // Now let's define the LeNet.  Broadly speaking, there are 3 parts to a network
    // definition.  The loss layer, a bunch of computational layers, and then an input
    // layer.  You can see these components in the network definition below.
    //
    // The input layer here says the network expects to be given matrix<unsigned char>
    // objects as input.  In general, you can use any dlib image or matrix type here, or
    // even define your own types by creating custom input layers.
    //
    // Then the middle layers define the computation the network will do to transform the
    // input into whatever we want.  Here we run the image through multiple convolutions,
    // ReLU units, max pooling operations, and then finally a fully connected layer that
    // converts the whole thing into just 10 numbers.
    //
    // Finally, the loss layer defines the relationship between the network outputs, our 10
    // numbers, and the labels in our dataset.  Since we selected loss_multiclass_log it
    // means we want to do multiclass classification with our network.   Moreover, the
    // number of network outputs (i.e. 10) is the number of possible labels.  Whichever
    // network output is largest is the predicted label.  So for example, if the first
    // network output is largest then the predicted digit is 0, if the last network output
    // is largest then the predicted digit is 9.
    using net_type = loss_multiclass_log<
                                fc<10,
                                relu<fc<84,
                                relu<fc<120,
                                max_pool<2,2,2,2,relu<con<16,5,5,1,1,
                                max_pool<2,2,2,2,relu<con<6,5,5,1,1,
                                input<matrix<unsigned char>>
                                >>>>>>>>>>>>;
    // This net_type defines the entire network architecture.  For example, the block
    // relu<fc<84,SUBNET>> means we take the output from the subnetwork, pass it through a
    // fully connected layer with 84 outputs, then apply ReLU.  Similarly, a block of
    // max_pool<2,2,2,2,relu<con<16,5,5,1,1,SUBNET>>> means we apply 16 convolutions with a
    // 5x5 filter size and 1x1 stride to the output of a subnetwork, then apply ReLU, then
    // perform max pooling with a 2x2 window and 2x2 stride.



    // So with that out of the way, we can make a network instance.
    net_type net;
    // Now if we later wanted to recall the network from disk we can simply say:
    deserialize("/mnist_network.mnist.dlib") >> net;

    // Let's also see if the network can correctly classify the testing images.  Since
    // MNIST is an easy dataset, we should see at least 99% accuracy.
    std::vector<unsigned long> predicted_labels = net(test_images);
    return predicted_labels[0];

    #endif
}
