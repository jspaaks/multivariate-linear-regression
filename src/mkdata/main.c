#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "boxmuller/boxmuller.h"
#include "matrix/matrix.h"
#include "options.h"

void show_usage (FILE * stream);
void populate_features (const Matrix * lower_bounds, const Matrix * upper_bounds, Matrix * features);
void populate_labels (const Matrix * true_weights, const Matrix * features, float sigma, Matrix * labels, Matrix * true_residuals);


int main (int argc, char * argv[]) {

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // =========== COLLECT USER INPUT AND INITIALIZE ARRAYS ================ //

    if (scan_for_help(argc, argv)) {
        show_usage(stdout);
        exit(EXIT_SUCCESS);
    }

    char basename[129] = "artificial-data.";
    size_t nfeatures = scan_for_nfeatures(argc, argv);
    size_t nsamples = scan_for_nsamples(argc, argv);
    float sigma = scan_for_sigma(argc, argv);
    scan_for_basename(argc, argv, basename);

    Matrix * features = matrix_create(nsamples, nfeatures);
    Matrix * labels = matrix_create(nsamples, 1);
    Matrix * lower_bounds = matrix_create(1, nfeatures);
    Matrix * true_residuals = matrix_create(nsamples, 1);
    Matrix * true_weights = matrix_create(1, 1 + nfeatures);
    Matrix * upper_bounds = matrix_create(1, nfeatures);

    scan_for_true_weights(argc, argv, nfeatures, features);
    scan_for_lower_bounds(argc, argv, nfeatures, lower_bounds);
    scan_for_upper_bounds(argc, argv, nfeatures, upper_bounds);

    fprintf(stdout, "nfeatures = %zu\n", nfeatures);
    fprintf(stdout, "nsamples = %zu\n", nsamples);
    fprintf(stdout, "sigma = %f\n", sigma);
    fprintf(stdout, "basename = \"%s\"\n", basename);

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
    fprintf(stream, "Usage: \n");
}