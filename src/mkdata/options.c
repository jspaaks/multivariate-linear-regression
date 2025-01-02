#include "matrix/matrix.h"
#include "options.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


void scan_for_basename (int argc, char * argv[], char * basename) {
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-b", 3) == 0;
        bool lng = strncmp(argv[i], "--basename", 11) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "parameter '--basename' is missing its value\n");
                exit(EXIT_FAILURE);
            }
            sscanf(argv[j], " %s", basename);
            break;
        }
    }
}


bool scan_for_help (int argc, char * argv[]) {
    for (int i = 0; i < argc; i++) {
        bool a = strncmp(argv[i], "-h", 3) == 0;
        bool b = strncmp(argv[i], "--help", 7) == 0;
        if (a || b) {
            return true;
        }
    }
    return false;
}


size_t scan_for_nfeatures (int argc, char * argv[]) {
    size_t nfeatures = 0;
    bool found = false;
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-d", 3) == 0;
        bool lng = strncmp(argv[i], "--nfeatures", 12) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "parameter '--nfeatures' is missing its value\n");
                exit(EXIT_FAILURE);
            }
            sscanf(argv[j], " %zu", &nfeatures);
            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "parameter '--nfeatures' is required\n");
        exit(EXIT_FAILURE);
    }
    return nfeatures;
}


size_t scan_for_nsamples (int argc, char * argv[]) {
    size_t nsamples = 0;
    bool found = false;
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-n", 3) == 0;
        bool lng = strncmp(argv[i], "--nsamples", 11) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "parameter '--nsamples' is missing its value\n");
                exit(EXIT_FAILURE);
            }
            sscanf(argv[j], " %zu", &nsamples);
            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "parameter '--nsamples' is required\n");
        exit(EXIT_FAILURE);
    }
    return nsamples;
}


float scan_for_sigma (int argc, char * argv[]) {
    float sigma = 1.0f;
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-s", 3) == 0;
        bool lng = strncmp(argv[i], "--sigma", 8) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "parameter '--sigma' is missing its value\n");
                exit(EXIT_FAILURE);
            }
            sscanf(argv[j], " %f", &sigma);
            break;
        }
    }
    return sigma;
}


void scan_for_lower_bounds (int argc, char * argv[], size_t nfeatures, Matrix * lower_bounds) {
    fprintf(stdout, "TODO - strtok lower_bounds\n");
    for (size_t i = 0; i < lower_bounds->n; i++) {
        lower_bounds->vals[i] = 0.0f;
    }
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-l", 3) == 0;
        bool lng = strncmp(argv[i], "--lower_bounds", 15) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "parameter '--lower_bounds' is missing its value\n");
                exit(EXIT_FAILURE);
            }

            // something with strtok
            // ...

            break;
        }
    }
}


void scan_for_true_weights (int argc, char * argv[], size_t nfeatures, Matrix * true_weights) {
    fprintf(stdout, "TODO - strtok true_weights\n");
    bool found = false;
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-w", 3) == 0;
        bool lng = strncmp(argv[i], "--true_weights", 15) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "parameter '--true_weights' is missing its value\n");
                exit(EXIT_FAILURE);
            }

            // something with strtok
            // ...

            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "parameter '--true_weights' is required\n");
        exit(EXIT_FAILURE);
    }
}


void scan_for_upper_bounds (int argc, char * argv[], size_t nfeatures, Matrix * upper_bounds) {
    fprintf(stdout, "TODO - strtok upper_bounds\n");
    for (size_t i = 0; i < upper_bounds->n; i++) {
        upper_bounds->vals[i] = 1.0f;
    }
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-u", 3) == 0;
        bool lng = strncmp(argv[i], "--upper_bounds", 15) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "parameter '--upper_bounds' is missing its value\n");
                exit(EXIT_FAILURE);
            }

            // something with strtok
            // ...

            break;
        }
    }
}
