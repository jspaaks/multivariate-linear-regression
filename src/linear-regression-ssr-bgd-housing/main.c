#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "data.h"
#include "lfuns/lfuns.h"
#include "ops/ops.h"

void print_usage (FILE *, char * []);

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

    // ============================= DATA ================================= //

    const size_t ni = 47;
    const size_t nf = 2;
    const size_t no = 1;
    const char * data_path = argv[1];

    Matrix * features = matrix_create(ni, 1 + nf);
    Matrix * features_avgs = matrix_create(1, 1 + nf);
    Matrix * features_stddevs = matrix_create(1, 1 + nf);
    Matrix * features_tr = matrix_create(1 + nf, ni);
    populate_features(data_path, features);
    matrix_stzdwn(features, features_avgs, features_stddevs, features);
    matrix_transp(features, features_tr);

    Matrix * labels = matrix_create(ni, no);
    Matrix * labels_avgs = matrix_create(1, no);
    Matrix * labels_stddevs = matrix_create(1, no);
    Matrix * labels_tr = matrix_create(no, ni);
    populate_labels(data_path, labels);
    matrix_stzdwn(labels, labels_avgs, labels_stddevs, labels);
    matrix_transp(labels, labels_tr);

    matrix_print(stdout, features, "features");
    matrix_print(stdout, labels, "labels");

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ======================= INITIALIZE ARRAYS ========================== //

    Matrix * weights = matrix_create(no, 1 + nf);
    Matrix * predicted = matrix_create(no, ni);
    Matrix * error = matrix_create(no, ni);
    Matrix * error_bcastd = matrix_create(1 + nf, ni);
    Matrix * gradients = matrix_create(1 + nf, ni);
    Matrix * gradient = matrix_create(1 + nf, no);
    Matrix * step = matrix_create(1 + nf, no);
    Matrix * step_tr = matrix_create(no, 1 + nf);
    Matrix * error_avg = matrix_create(no, 1);
    Matrix * answer = matrix_create(no, 1 + nf);
//
    size_t niters = 100;
    float learning_rate = 0.01f;
    for (size_t i = 0; i < niters; i++) {
        matrix_dotpro(weights, features_tr, predicted);
        matrix_ebesub(predicted, labels_tr, error);
        matrix_scapro(error, 2.0f, error);
        matrix_bctdwn(error, error_bcastd);
        matrix_hadpro(error_bcastd, features_tr, gradients);
        matrix_avgrgt(gradients, gradient);
        matrix_scapro(gradient, learning_rate, step);
        matrix_transp(step, step_tr);
        matrix_ebesub(weights, step_tr, weights);
    }
    matrix_avgrgt(error, error_avg);
    matrix_hadpro(weights, features_avgs, answer);
//
    matrix_print(stdout, weights, "weights");
    matrix_print(stdout, error_avg, "error_avg");
    matrix_print(stdout, answer, "answer");
//
//
    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //
//
    matrix_destroy(&error_avg);
    matrix_destroy(&step);
    matrix_destroy(&step_tr);
    matrix_destroy(&gradient);
    matrix_destroy(&gradients);
    matrix_destroy(&error_bcastd);
    matrix_destroy(&error);
    matrix_destroy(&predicted);
    matrix_destroy(&weights);
    matrix_destroy(&labels_tr);
    matrix_destroy(&labels_stddevs);
    matrix_destroy(&labels_avgs);
    matrix_destroy(&labels);
    matrix_destroy(&features_tr);
    matrix_destroy(&features_stddevs);
    matrix_destroy(&features_avgs);
    matrix_destroy(&features);


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
