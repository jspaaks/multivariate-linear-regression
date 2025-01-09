#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kwargs/kwargs.h"
#include "options.h"


void show_usage (FILE * stream);


int main (int argc, const char * argv[]) {

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // =========== COLLECT USER INPUT AND INITIALIZE ARRAYS ================ //

    const KwargsClass classes[] = {
        {
            .longname = "--help",
            .shortname = "-h",
            .type = KWARGS_FLAG
        },
        {
            .longname = "--learning_rate",
            .shortname = "-r",
            .type = KWARGS_OPTIONAL
        },
        {
            .longname = "--nepochs",
            .shortname = "-e",
            .type = KWARGS_OPTIONAL
        },
    };
    size_t nclasses = sizeof (classes) / sizeof (classes[0]);
    const Kwargs * kwargs = kwargs_create(argc, argv, nclasses, &classes[0]);
    if (kwargs_has_flag("--help", kwargs) > 0) {
        show_usage(stdout);
        goto deferred;
    }
    const float learning_rate = get_learning_rate(kwargs);
    const size_t nepochs = get_nepochs(kwargs);
    const char * features = kwargs_get_positional_value(0, kwargs);
    const char * labels = kwargs_get_positional_value(1, kwargs);

    fprintf(stdout, "learning_rate = %f\n", learning_rate);
    fprintf(stdout, "nepochs = %zu\n", nepochs);
    fprintf(stdout, "features = %s\n", features);
    fprintf(stdout, "labels = %s\n", labels);


deferred:
    kwargs_destroy((Kwargs **) &kwargs);
    return EXIT_SUCCESS;
}


void show_usage (FILE * stream) {
    char usage[] = "TODO\n";
    fprintf(stream, "%s", usage);
}
