#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "data.h"
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

    const size_t ni = 10000;
    const size_t nf = 2;

    Matrix * features = matrix_create(ni, 1 + nf);
    Matrix * features_transp = matrix_create(1 + nf, ni);
    Matrix * gradient = matrix_create(1 + nf, 1);
    Matrix * gradients = matrix_create(1 + nf, ni);
    Matrix * labels = matrix_create(ni, 1);
    Matrix * labels_transp = matrix_create(1, ni);
    Matrix * predicted = matrix_create(1, ni);
    Matrix * residuals = matrix_create(1, ni);
    Matrix * residuals_bcastd = matrix_create(1 + nf, ni);
    Matrix * step = matrix_create(1 + nf, 1);
    Matrix * step_transp = matrix_create(1, 1 + nf);
    Matrix * true_weights = matrix_create(1, 1 + nf);
    Matrix * weights = matrix_create(1, 1 + nf);

    // ============================= DATA ================================= //

    true_weights->vals[0] = 9.87;
    true_weights->vals[1] = 6.54;
    true_weights->vals[2] = 3.21;

    populate_features(features);
    matrix_transp(features, features_transp);
    matrix_print(stdout, features, "features");

    populate_labels (true_weights, features_transp, labels_transp);
    matrix_transp(labels_transp, labels);
    matrix_print(stdout, labels, "labels");

    size_t niters = 3800;
    float learning_rate = 0.01f;
    for (size_t i = 0; i < niters; i++) {
        matrix_dotpro(weights, features_transp, predicted);
        matrix_ebesub(predicted, labels_transp, residuals);
        matrix_bctdwn(residuals, residuals_bcastd);
        matrix_hadpro(residuals_bcastd, features_transp, gradients);
        matrix_avgrgt(gradients, gradient);
        matrix_scapro(gradient, learning_rate, step);
        matrix_transp(step, step_transp);
        matrix_ebesub(weights, step_transp, weights);
        if (i % 100 == 0) {
            fprintf(stdout, "======================= %zu =======================\n", i);
            matrix_print(stdout, weights, "weights");
            Matrix * residuals_avgrgt = matrix_create(1,1);
            matrix_avgrgt(residuals, residuals_avgrgt);
            matrix_print(stdout, residuals_avgrgt, "residuals_avgrgt");
            matrix_destroy(&residuals_avgrgt);
        }
    }

    //// =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    matrix_destroy(&features_transp);
    matrix_destroy(&features);
    matrix_destroy(&gradient);
    matrix_destroy(&gradients);
    matrix_destroy(&labels_transp);
    matrix_destroy(&labels);
    matrix_destroy(&predicted);
    matrix_destroy(&residuals_bcastd);
    matrix_destroy(&residuals);
    matrix_destroy(&step_transp);
    matrix_destroy(&step);
    matrix_destroy(&true_weights);
    matrix_destroy(&weights);

    return EXIT_SUCCESS;
}


void print_usage(FILE * stream, char * argv[]) {
    fprintf(stream,
            "Usage: %s\n"
            "    Generate 10000 random draws of points [-1, 1] in 2-D space, then\n"
            "    use true weights a = 9.87, b = 6.54, c = 3.21 to generate artificial\n"
            "    data according to z(x,y|a,b,c) = a + bx + cy. Then use batch gradient\n"
            "    descent to iteratively approximate the true value of the weights, by\n"
            "    minimizing the sum of squared residuals between artificial data and\n"
            "    predicted data.\n", argv[0]);
}
