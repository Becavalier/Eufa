#include "kmeans.h"


// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
    This is an example illustrating the use of the kkmeans object
    and spectral_cluster() routine from the dlib C++ Library.

    The kkmeans object is an implementation of a kernelized k-means clustering
    algorithm.  It is implemented by using the kcentroid object to represent
    each center found by the usual k-means clustering algorithm.

    So this object allows you to perform non-linear clustering in the same way
    a svm classifier finds non-linear decision surfaces.

    This example will make points from 3 classes and perform kernelized k-means
    clustering on those points.  It will also do the same thing using spectral
    clustering.

    The classes are as follows:
        - points very close to the origin
        - points on the circle of radius 10 around the origin
        - points that are on a circle of radius 4 but not around the origin at all
*/

const char* EMSCRIPTEN_KEEPALIVE testcase_kmeans(void) {
    // Here we declare that our samples will be 2 dimensional column vectors.
    // (Note that if you don't know the dimensionality of your vectors at compile time
    // you can change the 2 to a 0 and then set the size at runtime)
    typedef matrix<double, 2, 1> sample_type;

    // Now we are making a typedef for the kind of kernel we want to use.  I picked the
    // radial basis kernel because it only has one parameter and generally gives good
    // results without much fiddling.
    typedef radial_basis_kernel<sample_type> kernel_type;


    // Here we declare an instance of the kcentroid object.  It is the object used to
    // represent each of the centers used for clustering.  The kcentroid has 3 parameters
    // you need to set.  The first argument to the constructor is the kernel we wish to
    // use.  The second is a parameter that determines the numerical accuracy with which
    // the object will perform part of the learning algorithm.  Generally, smaller values
    // give better results but cause the algorithm to attempt to use more dictionary vectors
    // (and thus run slower and use more memory).  The third argument, however, is the
    // maximum number of dictionary vectors a kcentroid is allowed to use.  So you can use
    // it to control the runtime complexity.
    kcentroid<kernel_type> kc(kernel_type(0.1), 0.0001, 8);

    // Now we make an instance of the kkmeans object and tell it to use kcentroid objects
    // that are configured with the parameters from the kc object we defined above.
    kkmeans<kernel_type> test(kc);

    std::vector<sample_type> samples;
    std::vector<sample_type> initial_centers;

    sample_type m;

    dlib::rand rnd;

    // ASA058 the K-Means Problem testcases (http://people.sc.fsu.edu/~jburkardt/f_src/asa058/asa058.html)
    static double sample_x[] = {0.532095 ,0.189043 ,0.550977 ,0.003973 ,0.89423 ,0.073072 ,0.64018 ,0.389914 ,0.046918 ,0.820327 ,0.166575 ,0.587999 ,0.184717 ,0.264707 ,0.564959 ,0.986991 ,0.214221 ,0.997171 ,0.233578 ,0.209772 ,0.631619 ,0.441601 ,0.246242 ,0.243191 ,0.129101 ,0.808454 ,0.23591 ,0.841259 ,0.825533 ,0.780973 ,0.492706 ,0.064106 ,0.711451 ,0.532239 ,0.264527 ,0.984485 ,0.890788 ,0.81489 ,0.656357 ,0.161836 ,0.789622 ,0.005299 ,0.751558 ,0.315169 ,0.067572 ,0.144209 ,0.755242 ,0.025839 ,0.009056 ,0.091736 ,0.00012 ,0.599014 ,0.020924 ,0.402961 ,0.536965 ,0.791622 ,0.049268 ,0.321031 ,0.45994 ,0.306635 ,0.290264 ,0.117081 ,0.663268 ,0.25703 ,0.138396 ,0.37325 ,0.792062 ,0.761925 ,0.498106 ,0.079199 ,0.389481 ,0.909555 ,0.78771 ,0.87391 ,0.914291 ,0.126212 ,0.962689 ,0.662097 ,0.514842 ,0.573771 ,0.541641 ,0.880047 ,0.854456 ,0.882323 ,0.083516 ,0.868329 ,0.211671 ,0.169315 ,0.116072 ,0.900911 ,0.889781 ,0.209244 ,0.055621 ,0.787673 ,0.327654 ,0.571657 ,0.519674 ,0.020604 ,0.00635 ,0.414254};
    static double sample_y[] = {0.894141 ,0.613426 ,0.415724 ,0.60576 ,0.666812 ,0.740658 ,0.926186 ,0.553149 ,0.172275 ,0.578957 ,0.597895 ,0.824301 ,0.060804 ,0.661072 ,0.824897 ,0.654621 ,0.611877 ,0.807318 ,0.380796 ,0.585171 ,0.418295 ,0.474479 ,0.196578 ,0.428592 ,0.460463 ,0.240363 ,0.362678 ,0.018226 ,0.867529 ,0.282859 ,0.071775 ,0.024164 ,0.621806 ,0.872561 ,0.947361 ,0.373498 ,0.09006 ,0.765458 ,0.383494 ,0.878997 ,0.367808 ,0.694075 ,0.054149 ,0.989785 ,0.642346 ,0.130059 ,0.723929 ,0.306045 ,0.544864 ,0.031139 ,0.760615 ,0.406906 ,0.067692 ,0.743223 ,0.776167 ,0.4288 ,0.546021 ,0.883358 ,0.049388 ,0.920045 ,0.480864 ,0.709596 ,0.827229 ,0.908703 ,0.712536 ,0.578061 ,0.598336 ,0.679885 ,0.082325 ,0.879007 ,0.161374 ,0.33623 ,0.527877 ,0.282804 ,0.579771 ,0.635836 ,0.412397 ,0.205412 ,0.35217 ,0.571652 ,0.302552 ,0.447681 ,0.455932 ,0.006259 ,0.817145 ,0.54442 ,0.598359 ,0.4421 ,0.753312 ,0.049362 ,0.970528 ,0.783234 ,0.973298 ,0.077573 ,0.267293 ,0.956988 ,0.443726 ,0.472568 ,0.409455 ,0.229849};

    const long num = 100;

    for (long i = 0; i < num; ++i) {
        m(0) = sample_x[i];
        m(1) = sample_y[i];

        // add this sample to our set of samples we will run k-means
        samples.push_back(m);
    }

    // tell the kkmeans object we made that we want to run k-means with k set to 5.
    // (i.e. we want 5 clusters)
    test.set_number_of_centers(5);

    // You need to pick some initial centers for the k-means algorithm.  So here
    // we will use the dlib::pick_initial_centers() function which tries to find
    // n points that are far apart (basically).
    pick_initial_centers(5, initial_centers, samples, test.get_kernel());

    // now run the k-means algorithm on our set of samples.
    test.train(samples, initial_centers);

    // stringify to json for browser.
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartArray();
    for (unsigned long i = 0; i < samples.size(); ++i) {
        writer.StartArray();
        writer.Int(test(samples[i]));
        writer.Double(samples[i](0));
        writer.Double(samples[i](1));
        writer.EndArray();
    }
    writer.EndArray();
    return s.GetString();

    /*
    // now loop over all our samples and print out their predicted class.  In this example
    // all points are correctly identified.
    for (unsigned long i = 0; i < samples.size() / 3; ++i) {
        emscripten_log(EM_LOG_CONSOLE, "%d", test(samples[i]));
        emscripten_log(EM_LOG_CONSOLE, "%d", test(samples[i+num]));
        emscripten_log(EM_LOG_CONSOLE, "%d", test(samples[i+2*num]));
    }

    // Now print out how many dictionary vectors each center used.  Note that
    // the maximum number of 8 was reached.  If you went back to the kcentroid
    // constructor and changed the 8 to some bigger number you would see that these
    // numbers would go up.  However, 8 is all we need to correctly cluster this dataset.
    emscripten_log(EM_LOG_CONSOLE, "num dictionary vectors for center 0:  %d", test.get_kcentroid(0).dictionary_size());
    emscripten_log(EM_LOG_CONSOLE, "num dictionary vectors for center 1:  %d", test.get_kcentroid(1).dictionary_size());
    emscripten_log(EM_LOG_CONSOLE, "num dictionary vectors for center 2:  %d", test.get_kcentroid(2).dictionary_size());



    // Finally, we can also solve the same kind of non-linear clustering problem with
    // spectral_cluster().  The output is a vector that indicates which cluster each sample
    // belongs to.  Just like with kkmeans, it assigns each point to the correct cluster.
    std::vector<unsigned long> assignments = spectral_cluster(kernel_type(0.1), samples, 3);
    // cout << mat(assignments) << endl;

    */

}
