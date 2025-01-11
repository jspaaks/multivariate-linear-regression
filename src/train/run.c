#include "run.h"
#include "plotting.h"
#include <matrix/matrix.h>
#include <assert.h>


void make_standardized_related_features_matrices (const Matrix * features_r, Matrix * features_z, Matrix * features_zt1, Matrix * features_mu, Matrix * features_sigma);
void make_standardized_related_labels_matrices (const Matrix * labels_r, Matrix * labels_z, Matrix * labels_zt, Matrix * labels_mu, Matrix * labels_sigma);
void make_unstandardized_related_features_matrices (const Matrix * features_r, Matrix * features_rt1);
void make_unstandardized_related_labels_matrices (const Matrix * labels_r, Matrix * labels_rt);


void make_standardized_related_features_matrices (const Matrix * features_r, Matrix * features_z, Matrix * features_zt1, Matrix * features_mu, Matrix * features_sigma) {
    size_t nsamples = features_r->nr;
    size_t nfeatures = features_r->nc;
    Matrix * features_zt = matrix_create(nfeatures, nsamples);
    Matrix * ones = matrix_create(1, nsamples);
    matrix_ones(ones);
    matrix_stzdwn(features_r, features_mu, features_sigma, features_z);
    matrix_transp(features_z, features_zt);
    matrix_vstack(ones, features_zt, features_zt1);
    matrix_destroy(&features_zt);
    matrix_destroy(&ones);
}


void make_standardized_related_labels_matrices (const Matrix * labels_r, Matrix * labels_z, Matrix * labels_zt, Matrix * labels_mu, Matrix * labels_sigma) {
    matrix_stzdwn(labels_r, labels_mu, labels_sigma, labels_z);
    matrix_transp(labels_z, labels_zt);
}


void make_unstandardized_related_features_matrices (const Matrix * features_r, Matrix * features_rt1) {
    size_t nsamples = features_r->nr;
    size_t nfeatures = features_r->nc;
    Matrix * ones = matrix_create(1, nsamples);
    Matrix * features_rt = matrix_create(nfeatures, nsamples);
    matrix_ones(ones);
    matrix_transp(features_r, features_rt);
    matrix_vstack(ones, features_rt, features_rt1);
    matrix_destroy(&ones);
    matrix_destroy(&features_rt);
}


void make_unstandardized_related_labels_matrices (const Matrix * labels_r, Matrix * labels_rt) {
    matrix_transp(labels_r, labels_rt);
}


void run (const size_t nsamples, const size_t nfeatures, const size_t nepochs, const float learning_rate,
          const bool standardize, const bool verbose, const char * features_path, const char * labels_path) {

    fprintf(stderr, "Note: something is off with the backpropagation\n");

    // matrix suffixes: r raw, t transposed, 1 padded with ones
    Matrix * features_r = matrix_create(nsamples, nfeatures);
    Matrix * features = matrix_create(nsamples, nfeatures);
    Matrix * features_t1 = matrix_create(1 + nfeatures, nsamples);
    Matrix * features_mu = nullptr;
    Matrix * features_sigma = nullptr;
    if (standardize) {
        features_mu = matrix_create(1, nfeatures);
        features_sigma = matrix_create(1, nfeatures);
    }

    Matrix * labels_r = matrix_create(nsamples, 1);
    Matrix * labels = matrix_create(nsamples, 1);
    Matrix * labels_t = matrix_create(1, nsamples);
    Matrix * labels_mu = nullptr;
    Matrix * labels_sigma = nullptr;
    if (standardize) {
        labels_mu = matrix_create(1, 1);
        labels_sigma = matrix_create(1, 1);
    }

    Matrix * weights = matrix_create(1, 1 + nfeatures);
    Matrix * predicted_t = matrix_create(1, nsamples);
    Matrix * residuals_t = matrix_create(1, nsamples);
    Matrix * residuals_tb = matrix_create(1 + nfeatures, nsamples);
    Matrix * gradients_t = matrix_create(1 + nfeatures, nsamples);
    Matrix * gradient_t = matrix_create(1 + nfeatures, 1);
    Matrix * step_t = matrix_create(1 + nfeatures, 1);
    Matrix * step = matrix_create(1, 1 + nfeatures);

    Matrix * plotting_epochs = matrix_create(1 + nepochs, 1);
    Matrix * plotting_sigma = matrix_create(1 + nepochs, 1);

    // =====================  POPULATE DATA  ========================= //

    matrix_readxs(features_path, features_r);
    {
        size_t nrows = matrix_readnr(labels_path);
        size_t ncols = matrix_readnc(labels_path);
        assert(nrows == nsamples && "Expected the number of rows in FEATURES to be "
                                    "the same as the number of rows in LABELS");
        assert(ncols == 1 && "Expected the number of columns in LABELS to be 1");
    }
    matrix_readxs(labels_path, labels_r);

    if (standardize) {
        make_standardized_related_features_matrices(features_r, features, features_t1, features_mu, features_sigma);
        make_standardized_related_labels_matrices(labels_r, labels, labels_t, labels_mu, labels_sigma);
    } else {
        make_unstandardized_related_features_matrices(features_r, features_t1);
        make_unstandardized_related_labels_matrices(labels_r, labels_t);
    }
    if (verbose) matrix_print(stdout, "features_r", features_r);
    if (verbose) matrix_print(stdout, "features", features);
    if (verbose) matrix_print(stdout, "features_t1", features_t1);
    if (verbose) matrix_print(stdout, "labels_r", labels_r);
    if (verbose) matrix_print(stdout, "labels", labels);
    if (verbose) matrix_print(stdout, "labels_t", labels_t);

    // ========================== ITERATION ============================== //

    if (verbose) matrix_print(stdout, "weights", weights);

    for (size_t i = 0; i <= nepochs; i++) {
        if (verbose) fprintf(stdout, "+++++++++++++++++++++++++      epoch = %-5zu +++++++++++++++++++++++++\n", i);
        matrix_dotpro(weights, features_t1, predicted_t);
        if (verbose) matrix_print(stdout, "weights", weights);
        if (verbose) matrix_print(stdout, "features_t1", features_t1);
        if (verbose) matrix_print(stdout, "predicted_t", predicted_t);

        matrix_ebesub(predicted_t, labels_t, residuals_t);
        if (verbose) matrix_print(stdout, "residuals_t", residuals_t);

        {
            float halfssr = 0.0f;
            for (size_t ic = 0; ic < residuals_t->nc; ic++) {
                float x = residuals_t->xs[ic];
                halfssr += 0.5 * x * x;
            }
            plotting_epochs->xs[i] = i;
            plotting_sigma->xs[i] = halfssr;
        }
        matrix_bctdwn(residuals_t, residuals_tb);
        if (verbose) matrix_print(stdout, "residuals_tb", residuals_tb);

        matrix_hadpro(residuals_tb, features_t1, gradients_t);
        if (verbose) matrix_print(stdout, "gradients_t", gradients_t);

        matrix_accrgt(gradients_t, gradient_t);
        if (verbose) matrix_print(stdout, "gradient_t", gradient_t);

        matrix_scapro(gradient_t, learning_rate, step_t);
        if (verbose) matrix_print(stdout, "step_t", step_t);

        matrix_transp(step_t, step);
        if (verbose) matrix_print(stdout, "step", step);

        matrix_ebesub(weights, step, weights);
    }

    plot_residuals("qtwidget", plotting_epochs, plotting_sigma, nepochs, nsamples);

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    matrix_destroy(&features_r);
    matrix_destroy(&features);
    matrix_destroy(&features_t1);
    if (standardize) {
        matrix_destroy(&features_mu);
        matrix_destroy(&features_sigma);
    }

    matrix_destroy(&labels_r);
    matrix_destroy(&labels);
    matrix_destroy(&labels_t);
    if (standardize) {
        matrix_destroy(&labels_mu);
        matrix_destroy(&labels_sigma);
    }

    matrix_destroy(&weights);
    matrix_destroy(&predicted_t);
    matrix_destroy(&residuals_t);
    matrix_destroy(&residuals_tb);
    matrix_destroy(&gradients_t);
    matrix_destroy(&gradient_t);
    matrix_destroy(&step_t);
    matrix_destroy(&step);

    matrix_destroy(&plotting_epochs);
    matrix_destroy(&plotting_sigma);
}
