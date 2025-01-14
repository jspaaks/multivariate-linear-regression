#include "options.h"
#include "run.h"
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
    if (kwargs_requires_help(kwargs)) {
        show_usage(stdout);
        goto deferred;
    }
    const struct inputs inputs = {
        .nepochs = get_nepochs(kwargs),
        .learning_rate = get_learning_rate(kwargs),
        .standardize = kwargs_has_flag("--standardize", kwargs) > 0,
        .verbose = kwargs_has_flag("--verbose", kwargs) > 0,
        .features_path = kwargs_get_positional_value(0, kwargs),
        .labels_path = kwargs_get_positional_value(1, kwargs)
    };
    run(inputs);

deferred:
    kwargs_destroy((Kwargs **) &kwargs);
    return EXIT_SUCCESS;
}
