#include "options.h"
#include "standardized.h"
#include "unstandardized.h"
#include "kwargs/kwargs.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main (int argc, const char * argv[]) {

    const int npositionals = 2;
    if (argc <= npositionals) {
        show_usage(stderr);
        exit(EXIT_FAILURE);
    }

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ====================== COLLECT USER INPUT ========================== //

    const KwargsClass * classes = get_classes();
    const size_t nclasses = get_nclasses();
    const Kwargs * kwargs = kwargs_create(argc, argv, nclasses, &classes[0]);
    if (kwargs_has_flag("--help", kwargs) > 0) {
        show_usage(stdout);
        goto deferred;
    }
    const float learning_rate = get_learning_rate(kwargs);
    const size_t nepochs = get_nepochs(kwargs);
    const char * features_path = kwargs_get_positional_value(0, kwargs);
    const char * labels_path = kwargs_get_positional_value(1, kwargs);

    fprintf(stdout, "learning_rate = %f\n", learning_rate);
    fprintf(stdout, "nepochs = %zu\n", nepochs);
    fprintf(stdout, "features = %s\n", features_path);
    fprintf(stdout, "labels = %s\n", labels_path);

    // ========================== INITIALIZE ARRAYS ========================== //

    size_t nsamples = matrix_readnr(features_path);
    size_t nfeatures = matrix_readnc(features_path);

    if (kwargs_has_flag("--standardize", kwargs)) {
        run_standardized(nsamples, nfeatures, nepochs, learning_rate, features_path, labels_path);
    } else {
        run_unstandardized(nsamples, nfeatures, nepochs, learning_rate, features_path, labels_path);
    }


deferred:
    kwargs_destroy((Kwargs **) &kwargs);
    return EXIT_SUCCESS;
}
