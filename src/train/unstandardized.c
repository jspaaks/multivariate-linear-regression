#include "unstandardized.h"
#include <matrix/matrix.h>
#include <assert.h>


void make_related_features_matrices_unstandardized (Matrix * features, Matrix * features_t1);


void make_related_features_matrices_unstandardized (Matrix * features, Matrix * features_t1) {
    size_t nsamples = features->nr;
    size_t nfeatures = features->nc;
    Matrix * ones = matrix_create(1, nsamples);
    Matrix * features_t = matrix_create(nfeatures, nsamples);
    matrix_ones(ones);
    matrix_transp(features, features_t);
    matrix_vstack(ones, features_t, features_t1);
    matrix_destroy(&ones);
    matrix_destroy(&features_t);
}


void run_unstandardized(size_t nsamples, size_t nfeatures, size_t nepochs, float learning_rate, const char * features_path, const char * labels_path) {
    // suffixes t: transposed, 1: padded with ones
    Matrix * features = matrix_create(nsamples, nfeatures);
    Matrix * features_t1 = matrix_create(1 + nfeatures, nsamples);
    Matrix * labels = matrix_create(nsamples, 1);
    Matrix * labels_t = matrix_create(1, nsamples);

    Matrix * weights = matrix_create(1, 1 + nfeatures);
    Matrix * predicted = matrix_create(1, nsamples);
    Matrix * residuals = matrix_create(1, nsamples);
    Matrix * residuals_bctdwn = matrix_create(1 + nfeatures, nsamples);
    Matrix * gradients = matrix_create(1 + nfeatures, nsamples);
    Matrix * gradient = matrix_create(1 + nfeatures, 1);
    Matrix * step = matrix_create(1 + nfeatures, 1);
    Matrix * step_t = matrix_create(1, 1 + nfeatures);

    matrix_readxs(features_path, features);
    {
        size_t nrows = matrix_readnr(labels_path);
        size_t nlabels = matrix_readnc(labels_path);
        assert(nrows == nsamples && "Expected the number of rows in FEATURES to be "
                                    "the same as the number of rows in LABELS");
        assert(nlabels == 1 && "Expected the number of columns in LABELS to be 1");
    }
    matrix_readxs(labels_path, labels);

    make_related_features_matrices_unstandardized(features, features_t1);
    matrix_transp(labels, labels_t);

    matrix_print(stdout, "features_t1", features_t1);
    matrix_print(stdout, "labels_t", labels_t);

    // ========================== ITERATION ============================== //

    for (size_t i = 0; i <= nepochs; i++) {
        matrix_dotpro(weights, features_t1, predicted);
        matrix_ebesub(predicted, labels_t, residuals);
        matrix_bctdwn(residuals, residuals_bctdwn);
        matrix_hadpro(residuals_bctdwn, features_t1, gradients);
        matrix_avgrgt(gradients, gradient);
        matrix_scapro(gradient, learning_rate, step);
        matrix_transp(step, step_t);
        matrix_ebesub(weights, step_t, weights);
    }

    matrix_print(stdout, "weights", weights);

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    matrix_destroy(&features);
    matrix_destroy(&features_t1);
    matrix_destroy(&labels);
    matrix_destroy(&labels_t);

    matrix_destroy(&weights);
    matrix_destroy(&predicted);
    matrix_destroy(&residuals);
    matrix_destroy(&residuals_bctdwn);
    matrix_destroy(&gradients);
    matrix_destroy(&gradient);
    matrix_destroy(&step);
    matrix_destroy(&step_t);
}
