#include "options.h"
#include "run.h"
#include "plotting.h"
#include <matrix/matrix.h>
#include <assert.h>


/*
 * Matrix suffixes used herein:
 *        _1    padded with ones
 *        _b    broadcast
 *        _raw  raw
 *        _tr   transposed
 *        _z    standardized
 */


float calc_halfssr (Matrix * residuals_tr);
void make_standardized_related_features_matrices (const Matrix * features_raw, Matrix * features_z, Matrix * features_ztr1, Matrix * features_mu, Matrix * features_sigma);
void make_standardized_related_labels_matrices (const Matrix * labels_raw, Matrix * labels_z, Matrix * labels_ztr, Matrix * labels_mu, Matrix * labels_sigma);
void make_unstandardized_related_features_matrices (const Matrix * features_raw, Matrix * features, Matrix * features_rawtr1);
void make_unstandardized_related_labels_matrices (const Matrix * labels_raw, Matrix * labels, Matrix * labels_rawtr);


float calc_halfssr (Matrix * residuals_tr) {
    float halfssr = 0.0f;
    for (size_t ic = 0; ic < residuals_tr->nc; ic++) {
        float x = residuals_tr->xs[ic];
        halfssr += 0.5 * x * x;
    }
    return halfssr;
}


void make_standardized_related_features_matrices (const Matrix * features_raw, Matrix * features_z, Matrix * features_ztr1, Matrix * features_mu, Matrix * features_sigma) {
    size_t nsamples = features_raw->nr;
    size_t nfeatures = features_raw->nc;
    Matrix * features_ztr = matrix_create(nfeatures, nsamples);
    Matrix * ones = matrix_create(1, nsamples);
    matrix_ones(ones);
    matrix_stzdwn(features_raw, features_mu, features_sigma, features_z);
    matrix_transp(features_z, features_ztr);
    matrix_vstack(ones, features_ztr, features_ztr1);
    matrix_destroy(&features_ztr);
    matrix_destroy(&ones);
}


void make_standardized_related_labels_matrices (const Matrix * labels_raw, Matrix * labels_z, Matrix * labels_ztr, Matrix * labels_mu, Matrix * labels_sigma) {
    matrix_stzdwn(labels_raw, labels_mu, labels_sigma, labels_z);
    matrix_transp(labels_z, labels_ztr);
}


void make_unstandardized_related_features_matrices (const Matrix * features_raw, Matrix * features, Matrix * features_rawtr1) {
    matrix_addsca(features_raw, 0.0f, features);  // generate a copy
    size_t nsamples = features_raw->nr;
    size_t nfeatures = features_raw->nc;
    Matrix * ones = matrix_create(1, nsamples);
    Matrix * features_rawtr = matrix_create(nfeatures, nsamples);
    matrix_ones(ones);
    matrix_transp(features_raw, features_rawtr);
    matrix_vstack(ones, features_rawtr, features_rawtr1);
    matrix_destroy(&ones);
    matrix_destroy(&features_rawtr);
}


void make_unstandardized_related_labels_matrices (const Matrix * labels_raw, Matrix * labels, Matrix * labels_rawtr) {
    matrix_addsca(labels_raw, 0.0f, labels);  // generate a copy
    matrix_transp(labels_raw, labels_rawtr);
}


void run (const Kwargs * kwargs) {

    const char * device = options_get_device(kwargs);
    const char * features_path = options_get_features_path(kwargs);
    const char * labels_path = options_get_labels_path(kwargs);
    const float learning_rate = options_get_learning_rate(kwargs);
    const size_t nepochs = options_get_nepochs(kwargs);
    const bool standardize = options_get_standardize(kwargs);
    const bool verbose = options_get_verbose(kwargs);

    const size_t nsamples = matrix_readnr(features_path);
    const size_t nfeatures = matrix_readnc(features_path);

    if (verbose) {
        fprintf(stdout, "device = %s\n", device);
        fprintf(stdout, "features_path = %s\n", features_path);
        fprintf(stdout, "labels_path = %s\n", labels_path);
        fprintf(stdout, "learning_rate = %f\n", learning_rate);
        fprintf(stdout, "nepochs = %zu\n", nepochs);
        fprintf(stdout, "nfeatures = %zu\n", nfeatures);
        fprintf(stdout, "nsamples = %zu\n", nsamples);
        fprintf(stdout, "standardize = %s\n", standardize ? "true" : "false");
        fprintf(stdout, "verbose = %s\n", verbose ? "true" : "false");
    }

    Matrix * features_raw = matrix_create(nsamples, nfeatures);
    Matrix * features = matrix_create(nsamples, nfeatures);
    Matrix * features_tr1 = matrix_create(1 + nfeatures, nsamples);
    Matrix * features_mu = nullptr;
    Matrix * features_sigma = nullptr;
    if (standardize) {
        features_mu = matrix_create(1, nfeatures);
        features_sigma = matrix_create(1, nfeatures);
    }

    Matrix * labels_raw = matrix_create(nsamples, 1);
    Matrix * labels = matrix_create(nsamples, 1);
    Matrix * labels_tr = matrix_create(1, nsamples);
    Matrix * labels_mu = nullptr;
    Matrix * labels_sigma = nullptr;
    if (standardize) {
        labels_mu = matrix_create(1, 1);
        labels_sigma = matrix_create(1, 1);
    }

    Matrix * weights = matrix_create(1, 1 + nfeatures);
    Matrix * predicted_tr = matrix_create(1, nsamples);
    Matrix * residuals_tr = matrix_create(1, nsamples);
    Matrix * residuals_trb = matrix_create(1 + nfeatures, nsamples);
    Matrix * gradients_tr = matrix_create(1 + nfeatures, nsamples);
    Matrix * gradient_tr = matrix_create(1 + nfeatures, 1);
    Matrix * step_tr = matrix_create(1 + nfeatures, 1);
    Matrix * step = matrix_create(1, 1 + nfeatures);

    Matrix * epochs = matrix_create(1 + nepochs, 1);
    Matrix * losses = matrix_create(1 + nepochs, 1);

    // =====================  POPULATE DATA  ========================= //

    options_get_initial_weights(kwargs, weights);

    matrix_readxs(features_path, features_raw);
    {
        size_t nrows = matrix_readnr(labels_path);
        size_t ncols = matrix_readnc(labels_path);
        assert(nrows == nsamples && "Expected the number of rows in FEATURES to be "
                                    "the same as the number of rows in LABELS");
        assert(ncols == 1 && "Expected the number of columns in LABELS to be 1");
    }
    matrix_readxs(labels_path, labels_raw);

    if (standardize) {
        make_standardized_related_features_matrices(features_raw, features, features_tr1, features_mu, features_sigma);
        make_standardized_related_labels_matrices(labels_raw, labels, labels_tr, labels_mu, labels_sigma);
    } else {
        make_unstandardized_related_features_matrices(features_raw, features, features_tr1);
        make_unstandardized_related_labels_matrices(labels_raw, labels, labels_tr);
    }
    if (verbose) {
        matrix_print(stdout, "features_raw", features_raw);
        matrix_print(stdout, "features", features);
        matrix_print(stdout, "features_tr1", features_tr1);
        matrix_print(stdout, "labels_raw", labels_raw);
        matrix_print(stdout, "labels", labels);
        matrix_print(stdout, "labels_tr", labels_tr);
    }

    // ========================== ITERATION ============================== //

    if (verbose) matrix_print(stdout, "weights", weights);

    for (size_t i = 0; i <= nepochs; i++) {
        { // ==================== forward ==================== //
            matrix_dotpro(weights, features_tr1, predicted_tr);
        }
        { // ================ backpropagation ================ //
            matrix_ebesub(predicted_tr, labels_tr, residuals_tr);
            matrix_bctdwn(residuals_tr, residuals_trb);
            matrix_hadpro(residuals_trb, features_tr1, gradients_tr);
            matrix_accrgt(gradients_tr, gradient_tr);
            matrix_scapro(gradient_tr, learning_rate, step_tr);
            matrix_transp(step_tr, step);
            epochs->xs[i] = (float) i;
            losses->xs[i] = calc_halfssr(residuals_tr);
            if (verbose || i == nepochs) {
                fprintf(stdout, "+++++++++++++++++++++++++      epoch = %-5zu +++++++++++++++++++++++++\n", i);
                matrix_print(stdout, "weights", weights);
                matrix_print(stdout, "predicted_tr", predicted_tr);
                matrix_print(stdout, "residuals_tr", residuals_tr);
                matrix_print(stdout, "gradients_tr", gradients_tr);
                matrix_print(stdout, "step", step);
            }
            matrix_ebesub(weights, step, weights);
        }
    }

    plot_losses(device, nepochs, epochs, losses, nsamples);

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    matrix_destroy(&features_raw);
    matrix_destroy(&features);
    matrix_destroy(&features_tr1);
    if (standardize) {
        matrix_destroy(&features_mu);
        matrix_destroy(&features_sigma);
    }

    matrix_destroy(&labels_raw);
    matrix_destroy(&labels);
    matrix_destroy(&labels_tr);
    if (standardize) {
        matrix_destroy(&labels_mu);
        matrix_destroy(&labels_sigma);
    }

    matrix_destroy(&weights);
    matrix_destroy(&predicted_tr);
    matrix_destroy(&residuals_tr);
    matrix_destroy(&residuals_trb);
    matrix_destroy(&gradients_tr);
    matrix_destroy(&gradient_tr);
    matrix_destroy(&step_tr);
    matrix_destroy(&step);

    matrix_destroy(&epochs);
    matrix_destroy(&losses);
}
