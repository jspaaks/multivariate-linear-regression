#include "unstandardized.h"
#include "plotting.h"
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


void run_unstandardized(const size_t nsamples, const size_t nfeatures, const size_t nepochs, const float learning_rate,
                        const char * features_path, const char * labels_path) {
    // matrix suffixes: t transposed, 1 padded with ones
    Matrix * features = matrix_create(nsamples, nfeatures);
    Matrix * features_t1 = matrix_create(1 + nfeatures, nsamples);
    Matrix * labels = matrix_create(nsamples, 1);
    Matrix * labels_t = matrix_create(1, nsamples);

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

    matrix_readxs(features_path, features);
    make_related_features_matrices_unstandardized(features, features_t1);
    matrix_print(stdout, "features", features);
    matrix_print(stdout, "features_t1", features_t1);

    {
        size_t nrows = matrix_readnr(labels_path);
        size_t ncols = matrix_readnc(labels_path);
        assert(nrows == nsamples && "Expected the number of rows in FEATURES to be "
                                    "the same as the number of rows in LABELS");
        assert(ncols == 1 && "Expected the number of columns in LABELS to be 1");
    }
    matrix_readxs(labels_path, labels);
    matrix_transp(labels, labels_t);
    matrix_print(stdout, "labels", labels);
    matrix_print(stdout, "labels_t", labels_t);

    // ========================== ITERATION ============================== //

    for (size_t i = 0; i <= nepochs; i++) {
        matrix_dotpro(weights, features_t1, predicted_t);
        matrix_ebesub(predicted_t, labels_t, residuals_t);
        matrix_print(stdout, "weights", weights);
        matrix_print(stdout, "residuals_t", residuals_t);
        {
            plotting_epochs->xs[i] = i;
            plotting_sigma->xs[i] = matrix_sdvall(residuals_t);
        }
        matrix_bctdwn(residuals_t, residuals_tb);
        matrix_hadpro(residuals_tb, features_t1, gradients_t);
        matrix_avgrgt(gradients_t, gradient_t);
        matrix_scapro(gradient_t, learning_rate, step_t);
        matrix_transp(step_t, step);
        matrix_ebesub(weights, step, weights);
    }

    plot_residuals("qtwidget", plotting_epochs, plotting_sigma, nepochs, nsamples);

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    matrix_destroy(&features);
    matrix_destroy(&features_t1);
    matrix_destroy(&labels);
    matrix_destroy(&labels_t);

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
