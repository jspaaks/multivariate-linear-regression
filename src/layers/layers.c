#include "layers/layers.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void dot_product (const DotProductLayer * dpl, const float * input, float * output);


DotProductLayer * layers_create_dot_product_layer(size_t ni, size_t no) {
    float * w = calloc(ni * no, sizeof(float));
    if (w == nullptr) {
        fprintf(stderr, "Something went wrong allocating memory for DotProductLayer"
                        " instance's member 'weights'. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < ni * no; i++) {
        int z = rand() % 200 - 100;
        w[i] = (float) z / 1000;
    }
    float * g = calloc(ni * no, sizeof(float));
    if (g == nullptr) {
        fprintf(stderr, "Something went wrong allocating memory for DotProductLayer"
                        " instance's member 'gradients'. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    DotProductLayer * dpl = calloc(1, sizeof(DotProductLayer));
    if (dpl == nullptr) {
        fprintf(stderr, "Something went wrong allocating memory for DotProductLayer"
                        " instance. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    dpl->fwd = dot_product;
    dpl->ni = ni;
    dpl->no = no;
    dpl->weights = w;
    dpl->gradients = g;
    return dpl;
}


void layers_destroy_dot_product_layer(DotProductLayer ** dpl) {
    free((*dpl)->weights);
    (*dpl)->weights = nullptr;
    free((*dpl)->gradients);
    (*dpl)->gradients = nullptr;
    free(*dpl);
    *dpl = nullptr;
}


void dot_product (const DotProductLayer * dpl, const float * input, float * output) {
    size_t nr = dpl->no;
    size_t nc = dpl->ni;
    float * weights = dpl->weights;

    for (size_t ir = 0; ir < nr; ir++) {
        output[ir] = 0.0f;
        size_t o = ir * nc;
        for (size_t ic = 0; ic < nc; ic++) {
            output[ir] += weights[o+ic] * input[ic];
        }
    }
}
