#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "boxmuller/boxmuller.h"
#include "kwargs/kwargs.h"
#include "matrix/matrix.h"
#include "options.h"

void show_usage (FILE * stream);
void populate_features (const Matrix * lower_bounds, const Matrix * upper_bounds, Matrix * features);
void populate_labels (const Matrix * true_weights, const Matrix * features, float sigma, Matrix * labels, Matrix * true_residuals);


int main (int argc, const char * argv[]) {

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // =========== COLLECT USER INPUT AND INITIALIZE ARRAYS ================ //

    size_t nclasses = get_nclasses();
    const KwargsClass * classes = get_classes();
    size_t nclassifieds;
    KwargsType * classifieds = kwargs_create(argc);
    kwargs_classify(argc, argv, nclasses, classes, &nclassifieds, classifieds);
    if (kwargs_has_flag("--help", argc, argv, nclasses, classes, nclassifieds, classifieds) > 0) {
        show_usage(stdout);
        kwargs_destroy(&classifieds);
        exit(EXIT_SUCCESS);
    }
    size_t nfeatures = get_nfeatures(argc, argv, nclasses, classes, nclassifieds, classifieds);
    size_t nsamples = get_nsamples(argc, argv, nclasses, classes, nclassifieds, classifieds);
    float sigma = get_sigma(argc, argv, nclasses, classes, nclassifieds, classifieds);
    const char * basename = get_basename(argc, argv, nclasses, classes, nclassifieds, classifieds);

    Matrix * features = matrix_create(nsamples, nfeatures);
    Matrix * labels = matrix_create(nsamples, 1);
    Matrix * lower_bounds = matrix_create(1, nfeatures);
    Matrix * true_residuals = matrix_create(nsamples, 1);
    Matrix * true_weights = matrix_create(1, 1 + nfeatures);
    Matrix * upper_bounds = matrix_create(1, nfeatures);

    get_true_weights(argc, argv, nclasses, classes, nclassifieds, classifieds, true_weights, nfeatures);
    get_lower_bounds(argc, argv, nclasses, classes, nclassifieds, classifieds, lower_bounds, nfeatures);
    get_upper_bounds(argc, argv, nclasses, classes, nclassifieds, classifieds, upper_bounds, nfeatures);

    fprintf(stdout, "nfeatures = %zu\n", nfeatures);
    fprintf(stdout, "nsamples = %zu\n", nsamples);
    fprintf(stdout, "sigma = %f\n", sigma);
    fprintf(stdout, "basename = \"%s\"\n", basename);

    kwargs_destroy(&classifieds);

    // ===================== MAKE ARTIFICIAL DATA ========================= //

    populate_features(lower_bounds, upper_bounds, features);
    populate_labels(true_weights, features, sigma, labels, true_residuals);

    // ============================== IO ================================== //

    matrix_print(stdout, "lower_bounds", lower_bounds);
    matrix_print(stdout, "upper_bounds", upper_bounds);
    matrix_print(stdout, "true_weights", true_weights);
    matrix_print(stdout, "features", features);
    matrix_print(stdout, "labels", labels);
    matrix_print(stdout, "true_residuals", true_residuals);

    matrix_write(basename, "true_weights", true_weights);
    matrix_write(basename, "features", features);
    matrix_write(basename, "labels", labels);
    matrix_write(basename, "true_residuals", true_residuals);

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    matrix_destroy(&features);
    matrix_destroy(&labels);
    matrix_destroy(&lower_bounds);
    matrix_destroy(&true_residuals);
    matrix_destroy(&true_weights);
    matrix_destroy(&upper_bounds);

    return EXIT_SUCCESS;
}


void populate_features (const Matrix * lower_bounds, const Matrix * upper_bounds, Matrix * features) {
    assert(lower_bounds->nr == 1 && "Expected number of rows in lower bounds to be equal to 1");
    assert(upper_bounds->nr == 1 && "Expected number of rows in upper bounds to be equal to 1");
    assert(lower_bounds->nc == upper_bounds->nc && "Expected number of columns in lower bounds to be equal to number of columns in upper bounds");
    assert(features->nc == upper_bounds->nc && "Expected number of columns in features to be equal to number of columns in upper bounds");
    for ( size_t ic = 0; ic < lower_bounds->nc; ic++) {
        assert(lower_bounds->vals[ic] < upper_bounds->vals[ic] && "Expected lower bounds to be smaller than their corresponding upper bounds");
    }
    size_t nr = features->nr;
    size_t nc = features->nc;
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < nc; ic++) {
            size_t i = ir * nc + ic;
            float range = upper_bounds->vals[ic] - lower_bounds->vals[ic];
            float u = (float) rand() / RAND_MAX;
            features->vals[i] = lower_bounds->vals[ic] + u * range;
        }
    }
}


void populate_labels (const Matrix * true_weights, const Matrix * features, float sigma, Matrix * labels, Matrix * true_residuals) {
    size_t nsamples = features->nr;
    size_t nfeatures = features->nc;
    for (size_t isample = 0; isample < nsamples; isample++) {
        true_residuals->vals[isample] = (float) (sigma * boxmuller_randn());
    }
    Matrix * features_prime = matrix_create(nsamples, 1 + nfeatures);
    Matrix * features_prime_transp = matrix_create(1 + nfeatures, nsamples);
    Matrix * labels_transp = matrix_create(1, nsamples);
    Matrix * ones = matrix_create(nsamples, 1);
    matrix_ones(ones);
    matrix_hstack(ones, features, features_prime);
    matrix_transp(features_prime, features_prime_transp);
    matrix_dotpro(true_weights, features_prime_transp, labels_transp);
    matrix_transp(labels_transp, labels);
    matrix_ebeadd(labels, true_residuals, labels);
    matrix_destroy(&features_prime_transp);
    matrix_destroy(&features_prime);
    matrix_destroy(&labels_transp);
    matrix_destroy(&ones);
}


void show_usage (FILE * stream) {
    char usage[] = ""
        "NAME\n"
        "       mkdata - Generate multivariate linear artificial data\n"
        "\n"
        "SYNOPSIS\n"
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
