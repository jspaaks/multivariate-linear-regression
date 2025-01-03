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
                fprintf(stderr, "ERROR: parameter '--basename' is missing its value\n");
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
                fprintf(stderr, "ERROR: parameter '--nfeatures' is missing its value\n");
                exit(EXIT_FAILURE);
            }
            sscanf(argv[j], " %zu", &nfeatures);
            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "ERROR: parameter '--nfeatures' is required\n");
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
                fprintf(stderr, "ERROR: parameter '--nsamples' is missing its value\n");
                exit(EXIT_FAILURE);
            }
            sscanf(argv[j], " %zu", &nsamples);
            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "ERROR: parameter '--nsamples' is required\n");
        exit(EXIT_FAILURE);
    }
    return nsamples;
}


void scan_for_lower_bounds (int argc, char * argv[], size_t nfeatures, Matrix * lower_bounds) {
    for (size_t i = 0; i < lower_bounds->n; i++) {
        lower_bounds->vals[i] = 0.0f;
    }
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-l", 3) == 0;
        bool lng = strncmp(argv[i], "--lower_bounds", 15) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "ERROR: parameter '--lower_bounds' is missing its value\n");
                exit(EXIT_FAILURE);
            }

            size_t nfound = 0;
            char * token = nullptr;
            char * next = nullptr;
            char * str = argv[j];

            while (true) {
                token = strtok_r(str, ",", &next);
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
                str = nullptr;
            }
            if (nfound != nfeatures) {
                fprintf(stderr, "ERROR: Expected to sscanf %zu floating point values for parameter "
                                "'--lower_bounds', but found %zu\n", nfeatures, nfound);
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
}


float scan_for_sigma (int argc, char * argv[]) {
    float sigma = 1.0f;
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-s", 3) == 0;
        bool lng = strncmp(argv[i], "--sigma", 8) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "ERROR: parameter '--sigma' is missing its value\n");
                exit(EXIT_FAILURE);
            }
            sscanf(argv[j], " %f", &sigma);
            break;
        }
    }
    return sigma;
}


void scan_for_true_weights (int argc, char * argv[], size_t nfeatures, Matrix * true_weights) {
    bool found = false;
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-w", 3) == 0;
        bool lng = strncmp(argv[i], "--true_weights", 15) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "ERROR: parameter '--true_weights' is missing its value\n");
                exit(EXIT_FAILURE);
            }

            size_t nfound = 0;
            char * token = nullptr;
            char * next = nullptr;
            char * str = argv[j];

            while (true) {
                token = strtok_r(str, ",", &next);
                if (token == nullptr) {
                    break;
                }
                char remainder[128] = {};
                int nscanned = sscanf(token, " %f %127s ", &true_weights->vals[nfound], remainder);
                if (nscanned != 1) {
                    fprintf(stderr, "ERROR: Found trailing characters at index %zu when sscanf'ing "
                                    "floating point values from --true_weights\n", nfound);
                    exit(EXIT_FAILURE);
                }
                nfound++;
                str = nullptr;
            }
            if (nfound != nfeatures + 1) {
                fprintf(stderr, "ERROR: Expected to sscanf %zu floating point values for parameter "
                                "'--true_weights', but found %zu\n", nfeatures + 1, nfound);
                exit(EXIT_FAILURE);
            }

            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "ERROR: parameter '--true_weights' is required\n");
        exit(EXIT_FAILURE);
    }
}


bool scan_for_unauthorized (int argc, char * argv[]) {
    char * authorized[] = {
        "-d",
        "--nfeatures",
        "-n",
        "--nsamples",
        "-w",
        "--true_weights",
        "-b",
        "--basename",
        "-l",
        "--lower_bounds",
        "-s",
        "--sigma",
        "-u",
        "--upper_bounds",
        "-h",
        "--help"
        "-d",
        "--nfeatures",
        "-n",
        "--nsamples",
        "-w",
        "--true_weights",
        "-b",
        "--basename",
        "-l",
        "--lower_bounds",
        "-s",
        "--sigma",
        "-u",
        "--upper_bounds",
    };
    const size_t n = sizeof authorized / sizeof authorized[0];
    for (int i = 1; i < argc; i++) {
        char * arg = argv[i];
        bool is_shrt = strncmp(arg, "-", 1) == 0 && strlen(arg) == 2;
        bool is_long = strncmp(arg, "--", 2) == 0 && strlen(arg) > 2;
        bool is_key = is_shrt || is_long;
        if (!is_key) {
            continue; // arg is a value
        }
        bool found = false;
        for (size_t j = 0; j < n && !found; j++) {
            found = strcmp(arg, authorized[j]) == 0;
        }
        if (found) continue;
        fprintf(stdout, "Unauthorized option name \"%s\"\n.", arg);
        return true;
    }
    return false;
}


void scan_for_upper_bounds (int argc, char * argv[], size_t nfeatures, Matrix * upper_bounds) {
    for (size_t i = 0; i < upper_bounds->n; i++) {
        upper_bounds->vals[i] = 1.0f;
    }
    for (int i = 0; i < argc; i++) {
        bool shrt = strncmp(argv[i], "-u", 3) == 0;
        bool lng = strncmp(argv[i], "--upper_bounds", 15) == 0;
        if (shrt || lng) {
            size_t j = i + 1;
            if (j >= (size_t) argc) {
                fprintf(stderr, "ERROR: parameter '--upper_bounds' is missing its value\n");
                exit(EXIT_FAILURE);
            }

            size_t nfound = 0;
            char * token = nullptr;
            char * next = nullptr;
            char * str = argv[j];

            while (true) {
                token = strtok_r(str, ",", &next);
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
                str = nullptr;
            }
            if (nfound != nfeatures) {
                fprintf(stderr, "ERROR: Expected to sscanf %zu floating point values for parameter "
                                "'--upper_bounds', but found %zu\n", nfeatures, nfound);
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
}
