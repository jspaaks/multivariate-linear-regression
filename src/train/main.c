#include "options.h"
#include "run.h"
#include "kwargs/kwargs.h"
#include <stdlib.h>
#include <time.h>


int main (int argc, const char * argv[]) {

    const int npositionals = 2;
    if (argc <= npositionals) {
        options_show_usage(stderr);
        exit(EXIT_FAILURE);
    }

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ====================== COLLECT USER INPUT ========================== //

    const KwargsClass * classes = options_get_classes();
    const size_t nclasses = options_get_nclasses();
    const Kwargs * kwargs = kwargs_create(argc, argv, nclasses, &classes[0]);
    if (kwargs_requires_help(kwargs)) {
        options_show_usage(stdout);
        goto deferred;
    }
    run(kwargs);
deferred:
    kwargs_destroy((Kwargs **) &kwargs);
    return EXIT_SUCCESS;
}
