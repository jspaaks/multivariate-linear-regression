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
    },
    {
        .longname = "--initial_weights",
        .shortname = "-w",
        .type = KWARGS_OPTIONAL
    }
};


const KwargsClass * options_get_classes (void) {
    return &classes[0];
}


const char * options_get_features_path (const Kwargs * kwargs) {
    return kwargs_get_positional_value(0, kwargs);
}


void options_get_initial_weights (const Kwargs * kwargs, Matrix * weights) {
    const char * parameter_name = "--initial_weights";
    char * s = (char *) kwargs_get_optional_value (parameter_name, kwargs);
    if (s == nullptr) {
        for (size_t i = 0; i < weights->n; i++) {
            weights->xs[i] = 0.0f;
        }
        return;
    }
    size_t nfound = 0;
    char * token = nullptr;
    char * next = nullptr;
    for (size_t i = 0; i < weights->n; i++) {
        token = strtok_r(s, ",", &next);
        if (token == nullptr) break;
        char remainder[128] = {};
        int nscanned = sscanf(token, " %f %127s ", &weights->xs[i], remainder);
        if (nscanned != 1) {
            fprintf(stderr, "ERROR: Found trailing characters at index %zu when sscanf'ing "
                            "floating point values from '%s'\n", i, parameter_name);
            exit(EXIT_FAILURE);
        }
        nfound = i + 1;
        s = nullptr;  // (strtok_r magic)
    }
    if (nfound != weights->n) {
        fprintf(stderr, "ERROR: Expected to sscanf %zu floating point values for parameter "
                        "'%s', but found %zu\n", weights->n, parameter_name, nfound);
        exit(EXIT_FAILURE);
    }
}


const char * options_get_labels_path (const Kwargs * kwargs) {
    return kwargs_get_positional_value(1, kwargs);
}


float options_get_learning_rate (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value("--learning_rate", kwargs);
    if (s == nullptr) {
        return (float) 0.01;
    }
    float learning_rate;
    sscanf(s, " %f", &learning_rate);
    return learning_rate;
}


size_t options_get_nclasses (void) {
    return sizeof(classes) / sizeof(classes[0]);
}


size_t options_get_nepochs (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value("--nepochs", kwargs);
    if (s == nullptr) {
        return (size_t) 1;
    }
    size_t nepochs;
    sscanf(s, " %zu", &nepochs);
    return nepochs;
}


bool options_get_standardize (const Kwargs * kwargs) {
    return kwargs_has_flag("--standardize", kwargs) > 0;
}


bool options_get_verbose (const Kwargs * kwargs) {
    return kwargs_has_flag("--verbose", kwargs) > 0;
}


void options_show_usage (FILE * stream) {
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
                   "   -w, --initial_weights              Initial values of the weights (default all 0.0).\n"
                   "   -z, --standardize                  The learning rate (default 0.01).\n";
    fprintf(stream, "%s", usage);
}
