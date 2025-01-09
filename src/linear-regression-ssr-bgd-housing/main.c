#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "data.h"
#include "plotting.h"
#include "matrix/matrix.h"

void print_usage (FILE *, char * []);
void standardize_dwn(Matrix * features_raw, Matrix * features_raw_avg, Matrix * features_raw_sdv, Matrix * features);
void standardize_rgt(Matrix * labels_raw_transp, Matrix * labels_raw_transp_avgs, Matrix * labels_raw_transp_sdvs, Matrix * labels_transp);

int main (int argc, char * argv[]) {

    if (argc == 1) {
        print_usage(stderr, argv);
        exit(EXIT_FAILURE);
    }
    {
        bool a = strncmp(argv[1], "-h", 3) == 0;
        bool b = strncmp(argv[1], "--help", 7) == 0;
        if (a || b) {
            print_usage(stdout, argv);
            exit(EXIT_SUCCESS);
        }
    }

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ======================= INITIALIZE ARRAYS ========================== //

    const size_t ni = 47;
    const size_t nf = 2;
    const size_t nepochs = 1000;
    const float learning_rate = 0.01f;

    Matrix * features = matrix_create(ni, 1 + nf);
    Matrix * features_raw = matrix_create(ni, 1 + nf);
    Matrix * features_raw_avgs = matrix_create(1, 1 + nf);
    Matrix * features_raw_sdvs = matrix_create(1, 1 + nf);
    Matrix * features_transp = matrix_create(1 + nf, ni);
    Matrix * gradient = matrix_create(1 + nf, 1);
    Matrix * gradients = matrix_create(1 + nf, ni);
    Matrix * labels = matrix_create(ni, 1);
    Matrix * labels_transp = matrix_create(1, ni);
    Matrix * labels_raw_transp = matrix_create(1, ni);
    Matrix * labels_raw_transp_avgs = matrix_create(1, 1);
    Matrix * labels_raw_transp_sdvs = matrix_create(1, 1);
    Matrix * plotting_iterations = matrix_create(nepochs + 1, 1);
    Matrix * plotting_sigma = matrix_create(nepochs + 1, 1);
    Matrix * predicted = matrix_create(1, ni);
    Matrix * residuals = matrix_create(1, ni);
    Matrix * residuals_bctdwn = matrix_create(1 + nf, ni);
    Matrix * step = matrix_create(1 + nf, 1);
    Matrix * step_transp = matrix_create(1, 1 + nf);
    Matrix * true_residuals_transp = matrix_create(1, ni);
    Matrix * true_weights = matrix_create(1, 1 + nf);
    Matrix * weights = matrix_create(1, 1 + nf);

    // ============================= DATA ================================= //

    const char * data_path = argv[1];
    populate_features(data_path, features_raw);
    standardize_dwn(features_raw, features_raw_avgs, features_raw_sdvs, features);
    matrix_transp(features, features_transp);

    matrix_print(stdout, "features_raw", features_raw);
    matrix_print(stdout, "features_raw_avgs", features_raw_avgs);
    matrix_print(stdout, "features_raw_sdvs", features_raw_sdvs);
    matrix_print(stdout, "features", features);
    matrix_print(stdout, "features_transp", features_transp);

    populate_labels(data_path, labels_raw_transp);
    standardize_rgt(labels_raw_transp, labels_raw_transp_avgs, labels_raw_transp_sdvs, labels_transp);
    matrix_transp(labels_transp, labels);

    matrix_print(stdout, "labels_raw_transp", labels_raw_transp);
    matrix_print(stdout, "labels_raw_transp_avgs", labels_raw_transp_avgs);
    matrix_print(stdout, "labels_raw_transp_sdvs", labels_raw_transp_sdvs);
    matrix_print(stdout, "labels_transp", labels_transp);
    matrix_print(stdout, "labels", labels);

    // ========================== ITERATION ============================== //

    for (size_t i = 0; i <= nepochs; i++) {
        matrix_dotpro(weights, features_transp, predicted);
        matrix_ebesub(predicted, labels_transp, residuals);
        {
            plotting_iterations->xs[i] = i;
            plotting_sigma->xs[i] = matrix_sdvall(residuals);
        }
        matrix_bctdwn(residuals, residuals_bctdwn);
        matrix_hadpro(residuals_bctdwn, features_transp, gradients);
        matrix_avgrgt(gradients, gradient);
        matrix_scapro(gradient, learning_rate, step);
        matrix_transp(step, step_transp);
        matrix_ebesub(weights, step_transp, weights);
    }

    matrix_print(stdout, "weights", weights);
    plot_residuals("qtwidget", plotting_iterations, plotting_sigma, nepochs, ni);

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    matrix_destroy(&features_transp);
    matrix_destroy(&features);
    matrix_destroy(&features_raw);
    matrix_destroy(&gradient);
    matrix_destroy(&gradients);
    matrix_destroy(&labels_raw_transp);
    matrix_destroy(&labels_transp);
    matrix_destroy(&labels);
    matrix_destroy(&plotting_iterations);
    matrix_destroy(&plotting_sigma);
    matrix_destroy(&predicted);
    matrix_destroy(&residuals_bctdwn);
    matrix_destroy(&residuals);
    matrix_destroy(&step_transp);
    matrix_destroy(&step);
    matrix_destroy(&true_residuals_transp);
    matrix_destroy(&true_weights);
    matrix_destroy(&weights);

    return EXIT_SUCCESS;
}


void print_usage(FILE * stream, char * argv[]) {
    fprintf(stream,
            "Usage: %s HOUSING_DATA_PATH\n"
            "   Read housing data from HOUSING_DATA_PATH and subsequently\n"
            "   use these data to fit a multivariate linear regression\n"
            "   model using (1) the normal equation and (2) iterative\n"
            "   approximation with gradient descent.\n", argv[0]);
}


void standardize_dwn(Matrix * features_raw, Matrix * features_raw_avgs, Matrix * features_raw_sdvs, Matrix * features) {
    matrix_stzdwn(features_raw, features_raw_avgs, features_raw_sdvs, features);
    // undo scaling of the intercept column:
    for (size_t ir = 0; ir < features->nr; ir++) {
        size_t i = ir * features->nc;
        features->xs[i] = 1.0f;
    }
    features_raw_avgs->xs[0] = 1.0f;
    features_raw_sdvs->xs[0] = 0.0f;
}


void standardize_rgt(Matrix * labels_raw_transp, Matrix * labels_raw_transp_avgs, Matrix * labels_raw_transp_sdvs, Matrix * labels_transp) {
    matrix_stzrgt(labels_raw_transp, labels_raw_transp_avgs, labels_raw_transp_sdvs, labels_transp);
}
