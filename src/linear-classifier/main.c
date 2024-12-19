#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "data/data.h"
#include "lfuns/lfuns.h"
#include "ops/ops.h"

void print_usage (FILE *, char * []);
void populate_features (const char * path, Matrix * features);
void populate_labels (const char * path, Matrix * labels);


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
    Matrix * features_td = matrix_create(1 + nf, ni);
    populate_features(data_path, features);
    matrix_transpose(features, features_td);

    Matrix * labels = matrix_create(ni, no);
    Matrix * labels_td = matrix_create(no, ni);
    populate_labels(data_path, labels);
    matrix_transpose(labels, labels_td);

    matrix_print(stdout, features, "features");
    matrix_print(stdout, labels, "labels");

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ======================= INITIALIZE ARRAYS ========================== //

    Matrix * weights = matrix_create(no, 1 + nf);
    weights->vals[0] = 50.0f;
    weights->vals[1] = 0.4f;
    weights->vals[2] = 2.0f;


    Matrix * predicted = matrix_create(no, ni);
    Matrix * error = matrix_create(no, ni);
    Matrix * error_avg = matrix_create(no, 1);
    Matrix * error_tr = matrix_create(ni, no);
    Matrix * error_tr_bd = matrix_create(ni, 1 + nf);
    Matrix * tmp = matrix_create(ni, 1 + nf);
    Matrix * gradient = matrix_create(no, 1 + nf);
    Matrix * step = matrix_create(no, 1 + nf);

    size_t niters = 50000000;
    float learning_rate = 0.0000000001;
    for (size_t i = 0; i < niters; i++) {
        matrix_dotproduct(weights, features_td, predicted);
        matrix_subtract(predicted, labels_td, error);
        matrix_avg_per_row(error, error_avg);
        matrix_print(stdout, error_avg, "error_avg");
        matrix_transpose(error, error_tr);
        matrix_broadcast_right(error_tr, error_tr_bd);
        matrix_hadamardproduct(features, error_tr_bd, tmp);
        matrix_acc_per_col(tmp, gradient);
        matrix_multiply_scalar(gradient, -1 * learning_rate, step);
        matrix_add_(weights, step);
    }
    matrix_print(stdout, weights, "weights");
    matrix_print(stdout, error_avg, "error_avg");

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    matrix_destroy(&step);
    matrix_destroy(&gradient);
    matrix_destroy(&tmp);
    matrix_destroy(&error_tr_bd);
    matrix_destroy(&error_tr);
    matrix_destroy(&error_avg);
    matrix_destroy(&error);
    matrix_destroy(&predicted);
    matrix_destroy(&weights);
    matrix_destroy(&labels_td);
    matrix_destroy(&labels);
    matrix_destroy(&features_td);
    matrix_destroy(&features);


    return EXIT_SUCCESS;
}


void populate_features (const char * path, Matrix * features) {
    errno = 0;
    FILE * fp = fopen(path, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    constexpr size_t bufsize = 100;
    char buffer[bufsize] = {};
    for (size_t ir = 0; ir < features->nr; ir++) {
        float * intercept = &features->vals[ir * features->nc + 0];
        float * area      = &features->vals[ir * features->nc + 1];
        float * bedrooms  = &features->vals[ir * features->nc + 2];
        *intercept = 1.0f;
        fgets(buffer, bufsize, fp);
        sscanf(buffer, "%f,%f,%*f\n", area, bedrooms);
    }
    fclose(fp);
}


void populate_labels (const char * path, Matrix * labels) {
    errno = 0;
    FILE * fp = fopen(path, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    constexpr size_t bufsize = 100;
    char buffer[bufsize] = {};
    for (size_t ir = 0; ir < labels->nr; ir++) {
        float * price = &labels->vals[ir];
        fgets(buffer, bufsize, fp);
        sscanf(buffer, "%*f,%*f,%f\n", price);
    }
    fclose(fp);
}


void print_usage(FILE * stream, char * argv[]) {
    fprintf(stream,
            "Usage: %s HOUSING_DATA_PATH\n"
            "   Read housing data from HOUSING_DATA_PATH and subsequently\n"
            "   use these data to fit a multivariate linear regression\n"
            "   model using (1) the normal equation and (2) iterative\n"
            "   approximation with gradient descent.\n", argv[0]);
}
