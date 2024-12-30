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

int main (int argc, char * argv[]) {

    if (argc > 1) {
        print_usage(stdout, argv);
        bool a = strncmp(argv[1], "-h", 3) == 0;
        bool b = strncmp(argv[1], "--help", 7) == 0;
        if (a || b) {
            exit(EXIT_SUCCESS);
        } else {
            exit(EXIT_FAILURE);
        }
    }

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ======================= INITIALIZE ARRAYS ========================== //

    const size_t ni = 100;
    const size_t nf = 2;
    const size_t niters = 2500;
    const float learning_rate = 0.01f;

    Matrix * features = matrix_create(ni, 1 + nf);
    Matrix * features_transp = matrix_create(1 + nf, ni);
    Matrix * gradient = matrix_create(1 + nf, 1);
    Matrix * gradients = matrix_create(1 + nf, ni);
    Matrix * labels = matrix_create(ni, 1);
    Matrix * labels_transp = matrix_create(1, ni);
    Matrix * plotting_iterations = matrix_create(niters + 1, 1);
    Matrix * plotting_sigma = matrix_create(niters + 1, 1);
    Matrix * predicted = matrix_create(1, ni);
    Matrix * residuals = matrix_create(1, ni);
    Matrix * residuals_bctdwn = matrix_create(1 + nf, ni);
    Matrix * step = matrix_create(1 + nf, 1);
    Matrix * step_transp = matrix_create(1, 1 + nf);
    Matrix * true_residuals_transp = matrix_create(1, ni);
    Matrix * true_weights = matrix_create(1, 1 + nf);
    Matrix * weights = matrix_create(1, 1 + nf);

    // ============================= DATA ================================= //

    populate_true_weights(true_weights);
    populate_features(features);
    matrix_transp(features, features_transp);
    populate_labels (true_weights, features_transp, labels_transp, true_residuals_transp);
    matrix_transp(labels_transp, labels);

    // ========================== ITERATION ============================== //

    for (size_t i = 0; i <= niters; i++) {
        matrix_dotpro(weights, features_transp, predicted);
        matrix_ebesub(predicted, labels_transp, residuals);
        {
            plotting_iterations->vals[i] = i;
            plotting_sigma->vals[i] = matrix_sdvall(residuals);
        }
        matrix_bctdwn(residuals, residuals_bctdwn);
        matrix_hadpro(residuals_bctdwn, features_transp, gradients);
        matrix_avgrgt(gradients, gradient);
        matrix_scapro(gradient, learning_rate, step);
        matrix_transp(step, step_transp);
        matrix_ebesub(weights, step_transp, weights);
    }

    matrix_print(stdout, true_weights, "true_weights");
    matrix_print(stdout, weights, "weights");
    plot_residuals("qtwidget", plotting_iterations, plotting_sigma, niters, ni);

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    matrix_destroy(&features_transp);
    matrix_destroy(&features);
    matrix_destroy(&gradient);
    matrix_destroy(&gradients);
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
            "Usage: %s\n"
            "   Generate 100 random draws of points [-1, 1] in 2-D space, then use\n"
            "   true weights a = 98.7, b = 65.4, c = 32.1 to generate artificial\n"
            "   data according to z(x,y|a,b,c,e) = a + bx + cy + e, where e is drawn\n"
            "   from a random normal distribution with standard deviation equal to 10.\n"
            "   Then use batch gradient descent to iteratively approximate the true\n"
            "   value of the weights, by minimizing the sum of squared residuals\n"
            "   between artificial data and predicted data using 2500 iterations.\n", argv[0]);
}
