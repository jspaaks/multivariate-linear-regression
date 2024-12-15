#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "layers/layers.h"
#include "lfuns/lfuns.h"
#include "idx/idx.h"


void print_usage (FILE *, char * []);
void print_image (FILE *, const Data *, size_t);


int main (int argc, char * argv[]) {

    // ========================= PARSE ARGUMENTS ========================== //

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

    // ============================= READ DATA ============================ //

    const char * images_path = argv[1];
    const char * labels_path = argv[2];
    Data * images = (Data *) idx_read(images_path);
    Data * labels = (Data *) idx_read(labels_path);
    idx_print_meta(stdout, images);
    idx_print_meta(stdout, labels);

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ======================= INITIALIZE ARRAYS ========================== //

    constexpr size_t n0 = 784;  // input
    constexpr size_t n1 = 300;  // hidden 1
    constexpr size_t n2 = 100;  // hidden 2
    constexpr size_t n3 = 10;   // output

    DotProductLayer * dpl1 = layers_create_dot_product_layer(n0, n1);
    float hidden1[n1] = {0};
    DotProductLayer * dpl2 = layers_create_dot_product_layer(n1, n2);
    float hidden2[n2] = {0};
    DotProductLayer * dpl3 = layers_create_dot_product_layer(n2, n3);
    float output[n3] = {0};
    float loss = 0.0f;

    // ========================== FORWARD PASS ============================ //

    for (size_t iobj = 0; iobj < images->nobjs; iobj++) {
        size_t i = iobj * images->stride;
        dpl1->fwd(dpl1, &images->vals[i], &hidden1[0]);
        dpl2->fwd(dpl2, &hidden1[0], &hidden2[0]);
        dpl3->fwd(dpl3, &hidden2[0], &output[0]);
        loss += lfuns_svm(n3, &output[0], labels->vals[iobj]) / labels->nobjs;
    }

    // ========================= BACKWARD PASS ============================ //

    // TODO

    // =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    layers_destroy_dot_product_layer(&dpl1);
    layers_destroy_dot_product_layer(&dpl2);
    layers_destroy_dot_product_layer(&dpl3);
    idx_destroy_data(&images);
    idx_destroy_data(&labels);

    return EXIT_SUCCESS;
}


void print_image (FILE * stream, const Data * images, size_t iobj) {
    size_t nr = images->dimension_sizes[1];
    size_t nc = images->dimension_sizes[2];
    assert(iobj < images->nobjs && "Index out of range");
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < nc; ic++) {
            size_t i = iobj * nr * nc + ir * nc + ic;
            char ch = images->vals[i] == 0 ? ' ' : '.';
            if (ic == nc - 1) {
                fprintf(stream, " %c\n", ch);
            } else {
                fprintf(stream, " %c", ch);
            }
        }
    }
}


void print_usage(FILE * stream, char * argv[]) {
    fprintf(stream,
            "Usage: %s IMAGES_PATH LABELS_PATH\n"
            "   Read IDX formatted images from IMAGES_PATH and read the\n"
            "   corresponding IDX formatted labels from LABELS_PATH.\n"
            "   Subsequently use these data to train a neural network.\n", argv[0]);
}
