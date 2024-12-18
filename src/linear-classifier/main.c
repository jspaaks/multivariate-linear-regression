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
void populate_features_and_labels (const char * path, Matrix * features, Matrix * labels);


void populate_features_and_labels (const char * path, Matrix * features, Matrix * labels) {
    errno = 0;
    FILE * fp = fopen(path, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    const size_t n = features->nc;
    // by convention, add a first dummy feature and set it to 1 such
    // that the corresponding weight becomes the intercept term
    for (size_t i = 0; i < n; i++) {
        features->vals[0 * n + i] = 1.0f;
    }
    // now read the actual data
    constexpr size_t bufsize = 100;
    char buffer[bufsize] = {};
    for (size_t i = 0; i < n; i++) {
        float * area = &features->vals[1 * n + i];
        fgets(buffer, bufsize, fp);
        sscanf(buffer, "%f,%*f,%*f\n", area);
    }
    fseek(fp, 0, SEEK_SET);
    for (size_t i = 0; i < n; i++) {
        float * bedrooms = &features->vals[2 * n + i];
        fgets(buffer, bufsize, fp);
        sscanf(buffer, "%*f,%f,%*f\n", bedrooms);
    }
    fseek(fp, 0, SEEK_SET);
    for (size_t i = 0; i < n; i++) {
        float * price = &labels->vals[i];
        fgets(buffer, bufsize, fp);
        sscanf(buffer, "%*f,%*f,%f\n", price);
    }
    fclose(fp);
}


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
    Matrix * features = matrix_create(nf + 1, ni);
    Matrix * features_tr = matrix_create(ni, nf + 1);
    Matrix * labels = matrix_create(no, ni);
    const char * data_path = argv[1];
    populate_features_and_labels(data_path, features, labels);
    matrix_transpose(features, features_tr);

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ======================= INITIALIZE ARRAYS ========================== //

    Matrix * w = matrix_create(no, nf + 1);
    Matrix * predicted = matrix_create(no, ni);
    Matrix * error = matrix_create(no, ni);
    Matrix * step = matrix_create(no, nf + 1);

    // ========================== FORWARD PASS ============================ //

    size_t niters = 50;
    float learning_rate = 0.01;
    for (size_t i = 0; i < niters; i++) {
        matrix_dotproduct(w, features, predicted);
        matrix_subtract(predicted, labels, error);
        matrix_dotproduct(error, features_tr, step);
        matrix_scale_(step, -1 * learning_rate);
        matrix_add_(w, step);
    }

    // ========================= BACKWARD PASS ============================ //

    // TODO

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    //data_destroy(&output);
    //data_destroy(&w3);
    //data_destroy(&hidden2);
    //data_destroy(&w2);
    //data_destroy(&hidden1);
    //data_destroy(&w1);
    //data_destroy(&labels);
    //data_destroy(&images);

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
