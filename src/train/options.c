#include "options.h"
#include "kwargs/kwargs.h"
#include <string.h>
#include <stdlib.h>

static const KwargsClass classes[] = {
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
    {
        .longname = "--standardize",
        .shortname = "-z",
        .type = KWARGS_FLAG
    },
    {
        .longname = "--verbose",
        .shortname = "-v",
        .type = KWARGS_FLAG
    }
};


const KwargsClass * get_classes (void) {
    return &classes[0];
}


float get_learning_rate (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value("--learning_rate", kwargs);
    if (s == nullptr) {
        return (float) 0.01;
    }
    float learning_rate;
    sscanf(s, " %f", &learning_rate);
    return learning_rate;
}


size_t get_nepochs (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value("--nepochs", kwargs);
    if (s == nullptr) {
        return (size_t) 1;
    }
    size_t nepochs;
    sscanf(s, " %zu", &nepochs);
    return nepochs;
}


size_t get_nclasses (void) {
    return sizeof(classes) / sizeof(classes[0]);
}


void show_usage (FILE * stream) {
    char usage[] = "train [OPTIONS...] FEATURES LABELS\n"
                   "   Use gradient descent to train a multivariate linear model based\n"
                   "   on the features from filepath FEATURES and corresponding labels\n"
                   "   from filepath LABELS.\n"
                   "\n"
                   "   Options\n"
                   "   -e, --nepochs EPOCHS               The number of epochs (default 1).\n"
                   "   -h, --help                         Show the help and exit.\n"
                   "   -r, --learning_rate LEARNING_RATE  The learning rate (default 0.01).\n"
                   "   -v, --verbose                      Log intermediate values to STDOUT.\n"
                   "   -z, --standardize                  The learning rate (default 0.01).\n";
    fprintf(stream, "%s", usage);
}
