#include "options.h"
#include "run.h"
#include "kwargs/kwargs.h"
#include <stdlib.h>
#include <time.h>


int main (int argc, const char * argv[]) {

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ====================== COLLECT USER INPUT ========================== //

    const KwargsClass * classes = options_get_classes();
    const size_t nclasses = options_get_nclasses();
    const size_t npositionals = 2;
    const Kwargs * kwargs = kwargs_create(argc, argv, nclasses, &classes[0], npositionals);
    if (kwargs_requires_help(kwargs)) {
        options_show_usage(stdout);
        goto deferred;
    }
    int code = run(kwargs);
deferred:
    kwargs_destroy((Kwargs **) &kwargs);
    if (code == 0) return EXIT_SUCCESS;
    return EXIT_FAILURE;
}
