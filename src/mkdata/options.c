#include "options.h"
#include "kwargs/kwargs.h"
#include <string.h>
#include <stdlib.h>

static const KwargsClass classes[] = {
    {
        .longname = "--nfeatures",
        .shortname = "-d",
        .type = KWARGS_REQUIRED
    },
    {
        .longname = "--nsamples",
        .shortname = "-n",
        .type = KWARGS_REQUIRED
    },
    {
        .longname = "--true_weights",
        .shortname = "-w",
        .type = KWARGS_REQUIRED
    },
    {
        .longname = "--basename",
        .shortname = "-b",
        .type = KWARGS_OPTIONAL
    },
    {
        .longname = "--help",
        .shortname = "-h",
        .type = KWARGS_FLAG
    },
    {
        .longname = "--lower_bounds",
        .shortname = "-l",
        .type = KWARGS_OPTIONAL
    },
    {
        .longname = "--sigma",
        .shortname = "-s",
        .type = KWARGS_OPTIONAL
    },
    {
        .longname = "--upper_bounds",
        .shortname = "-u",
        .type = KWARGS_OPTIONAL
    }
};


const char * get_basename (Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value ("--basename", kwargs);
    if (s == nullptr) {
        return "artificial-data.";
    }
    return s;
}


const KwargsClass * get_classes (void) {
    return &classes[0];
}


void get_lower_bounds (Kwargs * kwargs, Matrix * lower_bounds, size_t nfeatures) {
    char * s = (char *) kwargs_get_optional_value ("--lower_bounds", kwargs);
    if (s == nullptr) {
        for (size_t i = 0; i < lower_bounds->n; i++) {
            lower_bounds->vals[i] = 0.0f;
        }
        return;
    }

    size_t nfound = 0;
    char * token = nullptr;
    char * next = nullptr;

    while (true) {
        token = strtok_r(s, ",", &next);
        if (token == nullptr) {
            break;
        }
        char remainder[128] = {};
        int nscanned = sscanf(token, " %f %127s ", &lower_bounds->vals[nfound], remainder);
        if (nscanned != 1) {
            fprintf(stderr, "ERROR: Found trailing characters at index %zu when sscanf'ing "
                            "floating point values from --lower_bounds\n", nfound);
            exit(EXIT_FAILURE);
        }
        nfound++;
        s = nullptr;  // (strtok_r magic)
    }
    if (nfound != nfeatures) {
        fprintf(stderr, "ERROR: Expected to sscanf %zu floating point values for parameter "
                        "'--lower_bounds', but found %zu\n", nfeatures, nfound);
        exit(EXIT_FAILURE);
    }
}


size_t get_nclasses (void) {
    return sizeof (classes) / sizeof (classes[0]);
}


size_t get_nfeatures (Kwargs * kwargs) {
    const char * s = kwargs_get_required_value ("--nfeatures", kwargs);
    size_t nfeatures;
    sscanf(s, " %zu", &nfeatures);
    return nfeatures;
}


size_t get_nsamples (Kwargs * kwargs) {
    const char * s = kwargs_get_required_value ("--nsamples", kwargs);
    size_t nsamples;
    sscanf(s, " %zu", &nsamples);
    return nsamples;
}


float get_sigma (Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value ("--sigma", kwargs);
    if (s == nullptr) {
        return (float) 1;
    }
    float sigma;
    sscanf(s, " %f", &sigma);
    return sigma;
}


void get_true_weights (Kwargs * kwargs, Matrix * true_weights, size_t nfeatures) {
    char * s = (char *) kwargs_get_required_value ("--true_weights", kwargs);

    size_t nfound = 0;
    char * token = nullptr;
    char * next = nullptr;

    while (true) {
        token = strtok_r(s, ",", &next);
        if (token == nullptr) break;
        char remainder[128] = {};
        int nscanned = sscanf(token, " %f %127s ", &true_weights->vals[nfound], remainder);
        if (nscanned != 1) {
            fprintf(stderr, "ERROR: Found trailing characters at index %zu when sscanf'ing "
                            "floating point values from --true_weights\n", nfound);
            exit(EXIT_FAILURE);
        }
        nfound++;
        s = nullptr;  // (strtok_r magic)
    }
    if (nfound != nfeatures + 1) {
        fprintf(stderr, "ERROR: Expected to sscanf %zu floating point values for parameter "
                        "'--true_weights', but found %zu\n", nfeatures + 1, nfound);
        exit(EXIT_FAILURE);
    }
}

void get_upper_bounds (Kwargs * kwargs, Matrix * upper_bounds, size_t nfeatures) {
    char * s = (char *) kwargs_get_optional_value ("--upper_bounds", kwargs);
    if (s == nullptr) {
        for (size_t i = 0; i < upper_bounds->n; i++) {
            upper_bounds->vals[i] = 1.0f;
        }
        return;
    }

    size_t nfound = 0;
    char * token = nullptr;
    char * next = nullptr;

    while (true) {
        token = strtok_r(s, ",", &next);
        if (token == nullptr) {
            break;
        }
        char remainder[128] = {};
        int nscanned = sscanf(token, " %f %127s ", &upper_bounds->vals[nfound], remainder);
        if (nscanned != 1) {
            fprintf(stderr, "ERROR: Found trailing characters at index %zu when sscanf'ing "
                            "floating point values from --upper_bounds\n", nfound);
            exit(EXIT_FAILURE);
        }
        nfound++;
        s = nullptr;  // (strtok_r magic)
    }
    if (nfound != nfeatures) {
        fprintf(stderr, "ERROR: Expected to sscanf %zu floating point values for parameter "
                        "'--upper_bounds', but found %zu\n", nfeatures, nfound);
        exit(EXIT_FAILURE);
    }
}
