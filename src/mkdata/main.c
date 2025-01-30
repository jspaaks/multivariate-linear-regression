#include "options.h"
#include "boxmuller/boxmuller.h"
#include "kwargs/kwargs.h"
#include "matrix/matrix.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void populate_features (const Matrix * lower_bounds, const Matrix * upper_bounds, Matrix * features);
void populate_labels (const Matrix * true_weights, const Matrix * features, float sigma, Matrix * labels, Matrix * true_residuals);


int main (int argc, const char * argv[]) {

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // =========== COLLECT USER INPUT AND INITIALIZE ARRAYS ================ //

    const size_t nclasses = options_get_nclasses();
    const KwargsClass * classes = options_get_classes();
    const size_t npositionals = 0;
    const Kwargs * kwargs = kwargs_create(argc, argv, nclasses, classes, npositionals);
    if (kwargs_requires_help(kwargs)) {
        options_show_usage(stdout);
        kwargs_destroy((Kwargs **) &kwargs);
        exit(EXIT_SUCCESS);
    }
    const size_t nfeatures = options_get_nfeatures(kwargs);
    const size_t nsamples = options_get_nsamples(kwargs);
    const float sigma = options_get_sigma(kwargs);
    const char * basename = options_get_basename(kwargs);
    const bool verbose = options_get_verbose(kwargs);

    Matrix * features = matrix_create(nsamples, nfeatures);
    Matrix * labels = matrix_create(nsamples, 1);
    Matrix * lower_bounds = matrix_create(1, nfeatures);
    Matrix * true_residuals = matrix_create(nsamples, 1);
    Matrix * true_weights = matrix_create(1, 1 + nfeatures);
    Matrix * upper_bounds = matrix_create(1, nfeatures);

    options_get_true_weights(kwargs, true_weights, nfeatures);
    options_get_lower_bounds(kwargs, lower_bounds, nfeatures);
    options_get_upper_bounds(kwargs, upper_bounds, nfeatures);

    if (verbose) {
        fprintf(stdout, "nfeatures = %zu\n", nfeatures);
        fprintf(stdout, "nsamples = %zu\n", nsamples);
        fprintf(stdout, "sigma = %f\n", sigma);
        fprintf(stdout, "basename = \"%s\"\n", basename);
    }

    kwargs_destroy((Kwargs **) &kwargs);

    // ===================== MAKE ARTIFICIAL DATA ========================= //

    populate_features(lower_bounds, upper_bounds, features);
    populate_labels(true_weights, features, sigma, labels, true_residuals);

    // ============================== IO ================================== //

    if (verbose) {
        matrix_print(stdout, "lower_bounds", lower_bounds);
        matrix_print(stdout, "upper_bounds", upper_bounds);
        matrix_print(stdout, "true_weights", true_weights);
        matrix_print(stdout, "features", features);
        matrix_print(stdout, "labels", labels);
        matrix_print(stdout, "true_residuals", true_residuals);
    }

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
    for (size_t ic = 0; ic < lower_bounds->nc; ic++) {
        assert(lower_bounds->xs[ic] < upper_bounds->xs[ic] && "Expected lower bounds to be smaller than their corresponding upper bounds");
    }
    const size_t nr = features->nr;
    const size_t nc = features->nc;
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < nc; ic++) {
            const size_t i = ir * nc + ic;
            const float range = upper_bounds->xs[ic] - lower_bounds->xs[ic];
            const float u = (float) rand() / RAND_MAX;
            features->xs[i] = lower_bounds->xs[ic] + u * range;
        }
    }
}


void populate_labels (const Matrix * true_weights, const Matrix * features, const float sigma, Matrix * labels, Matrix * true_residuals) {
    const size_t nsamples = features->nr;
    const size_t nfeatures = features->nc;
    for (size_t isample = 0; isample < nsamples; isample++) {
        true_residuals->xs[isample] = (float) (sigma * boxmuller_randn());
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
