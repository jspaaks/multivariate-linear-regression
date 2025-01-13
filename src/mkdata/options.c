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


const char * get_basename (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value ("--basename", kwargs);
    if (s == nullptr) {
        return "artificial-data.";
    }
    return s;
}


const KwargsClass * get_classes (void) {
    return &classes[0];
}


void get_lower_bounds (const Kwargs * kwargs, Matrix * lower_bounds, size_t nfeatures) {
    char * s = (char *) kwargs_get_optional_value ("--lower_bounds", kwargs);
    if (s == nullptr) {
        for (size_t i = 0; i < lower_bounds->n; i++) {
            lower_bounds->xs[i] = 0.0f;
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
        int nscanned = sscanf(token, " %f %127s ", &lower_bounds->xs[nfound], remainder);
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


size_t get_nfeatures (const Kwargs * kwargs) {
    const char * s = kwargs_get_required_value ("--nfeatures", kwargs);
    size_t nfeatures;
    sscanf(s, " %zu", &nfeatures);
    return nfeatures;
}


size_t get_nsamples (const Kwargs * kwargs) {
    const char * s = kwargs_get_required_value ("--nsamples", kwargs);
    size_t nsamples;
    sscanf(s, " %zu", &nsamples);
    return nsamples;
}


float get_sigma (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value ("--sigma", kwargs);
    if (s == nullptr) {
        return (float) 1;
    }
    float sigma;
    sscanf(s, " %f", &sigma);
    return sigma;
}


void get_true_weights (const Kwargs * kwargs, Matrix * true_weights, size_t nfeatures) {
    char * s = (char *) kwargs_get_required_value ("--true_weights", kwargs);

    size_t nfound = 0;
    char * token = nullptr;
    char * next = nullptr;

    while (true) {
        token = strtok_r(s, ",", &next);
        if (token == nullptr) break;
        char remainder[128] = {};
        int nscanned = sscanf(token, " %f %127s ", &true_weights->xs[nfound], remainder);
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

void get_upper_bounds (const Kwargs * kwargs, Matrix * upper_bounds, size_t nfeatures) {
    char * s = (char *) kwargs_get_optional_value ("--upper_bounds", kwargs);
    if (s == nullptr) {
        for (size_t i = 0; i < upper_bounds->n; i++) {
            upper_bounds->xs[i] = 1.0f;
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
        int nscanned = sscanf(token, " %f %127s ", &upper_bounds->xs[nfound], remainder);
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


void show_usage (FILE * stream) {
    char usage[] = ""
        "NAME\n"
        "       mkdata - Generate multivariate linear artificial data\n"
        "\n"
        "SYNOPSIS\n"
        "       mkdata -h\n"
        "       mkdata --help\n"
        "       mkdata [OPTION]... [REQUIRED]...\n"
        "\n"
        "DESCRIPTION\n"
        "       Required\n"
        "\n"
        "       -d NFEATURES, --nfeatures NFEATURES\n"
        "            Number  of  features,  i.e. the dimensionality of the data\n"
        "            set. Its corresponding value NFEATURES should be  a  posi‐\n"
        "            tive integer.\n"
        "\n"
        "       -n NSAMPLES, --nsamples NSAMPLES\n"
        "            Number  of  samples, i.e. the number of points in n-dimen‐\n"
        "            sional space that the generated data set will  contain,  n\n"
        "            being  the  number  of  features.  Its corresponding value\n"
        "            NSAMPLES should be a positive integer.\n"
        "\n"
        "       -w TRUE_WEIGHTS, --true_weights TRUE_WEIGHTS\n"
        "            True weights. The true coefficients  of  the  multivariate\n"
        "            linear  equation  that  is used to generate the artificial\n"
        "            data set. The first weight represents the  intercept.  Its\n"
        "            corresponding  value  TRUE_WEIGHTS should be a comma-sepa‐\n"
        "            rated, double-quoted  string,  containing  1  +  NFEATURES\n"
        "            floating point numbers.\n"
        "\n"
        "       Options\n"
        "\n"
        "       -b BASENAME, --basename BASENAME\n"
        "            Basename of the output files. Any directory names in BASE‐\n"
        "            NAME  should exist before running the command. If this op‐\n"
        "            tion is omitted, BASENAME is set to  the  string  \"artifi‐\n"
        "            cial-data.\".\n"
        "\n"
        "       -h, --help\n"
        "            Show the help and exit.\n"
        "\n"
        "       -l LOWER_BOUNDS, --lower_bounds LOWER_BOUNDS\n"
        "            Lower bounds of the range of each feature. Its correspond‐\n"
        "            ing  value  LOWER_BOUNDS should be a comma-separated, dou‐\n"
        "            ble-quoted string,  containing  NFEATURES  floating  point\n"
        "            numbers.  If  this  option  is  omitted,  each  element of\n"
        "            LOWER_BOUNDS is set to 0.0f;\n"
        "\n"
        "       -s SIGMA, --sigma SIGMA\n"
        "            Standard deviation of  the  residuals.  Its  corresponding\n"
        "            value SIGMA should be a floating point number. If this op‐\n"
        "            tion is omitted, SIGMA is set to 1.0f.\n"
        "\n"
        "       -u UPPER_BOUNDS, --upper_bounds UPPER_BOUNDS\n"
        "            Upper bounds of the range of each feature. Its correspond‐\n"
        "            ing  value  UPPER_BOUNDS should be a comma-separated, dou‐\n"
        "            ble-quoted string,  containing  NFEATURES  floating  point\n"
        "            numbers.  If  this  option is omitted, each element of UP‐\n"
        "            PER_BOUNDS is set to 1.0f;\n"
        "\n"
        "EXAMPLES\n"
        "       Generate 7 samples in a 3-dimensional space, using the  default\n"
        "       bounds, standard deviation, and basename:\n"
        "\n"
        "            $ mkdata --nfeatures 3 --nsamples 7 --true_weights \"8.7,2.34,23.0,150\"\n"
        "\n"
        "       Generate 25 samples in  a  2-dimensional  space,  using  custom\n"
        "       bounds, standard deviation, and basename:\n"
        "\n"
        "            $ mkdir data && mkdata --nfeatures 2 \\\n"
        "                                   --nsamples 25 \\\n"
        "                                   --true_weights \"246.5,78.35,68.0\" \\\n"
        "                                   --lower_bounds \"-10.0,1000\" \\\n"
        "                                   --upper_bounds \"10.0,3000\" \\\n"
        "                                   --sigma 12.1 \\\n"
        "                                   --basename \"data/\"\n";
    fprintf(stream, "%s", usage);
}
