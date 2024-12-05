#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ann/network.h"
#include "idxread/idxread.h"


void seed_network (Network *);
void print_usage (FILE *, char * []);
void print_image (FILE *, const Meta *, const uint8_t *, size_t);
float * scale_then_translate(Meta *, uint8_t *);


static uint8_t scale = 1;
static uint8_t translation = 0;


int main (int argc, char * argv[]) {

    if (argc == 1) {
        print_usage(stderr, argv);
        exit(EXIT_FAILURE);
    }
    if (strncmp(argv[1], "-h", 3) == 0 || strncmp(argv[1], "--help", 7) == 0) {
        print_usage(stdout, argv);
        exit(EXIT_SUCCESS);
    }

    // ============================================================ //

    char * path = argv[1];
    Meta meta = {0};
    idxread__get_meta(path, &meta);
    uint8_t * raw = idxread__get_data(path, &meta);
    idxread__print_meta(stdout, &meta);
    print_image(stdout, &meta, &raw[0], 0);
    float * data = scale_then_translate(&meta, &raw[0]);

    // ============================================================ //

    constexpr size_t nlayers = 4;
    size_t nnodes[nlayers] = {784, 300, 100, 10};
    srand(time(nullptr));
    Network * network = ann__network_create(nlayers, nnodes);
    ann__network_print(stdout, network);

    const size_t niter = 10;
    const float learning_rate = 0.01;
    for (size_t iiter = 0; iiter < niter; iiter++) {
        ann__network_populate_weights(network);
        ann__network_populate_biases(network);
        for (size_t iobj = 0; iobj < meta.nobjs; iobj++) {
            ann__network_populate_input(network, &meta, &data[0], iobj);
            ann__network_fwdpass(network);
            // ann__calc_loss();
        }
        ann__network_backprop(network, learning_rate);
    }

    ann__network_destroy(network);

    free(raw);
    free(data);

    return 0;
}


void print_image (FILE * stream, const Meta * meta, const uint8_t * data, size_t iobj) {
    size_t nr = meta->dimension_sizes[1];
    size_t nc = meta->dimension_sizes[2];
    assert(iobj < meta->nobjs && "Index out of range");
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < nc; ic++) {
            size_t i = iobj * nr * nc + ir * nc + ic;
            char ch = data[i] == 0 ? ' ' : '.';
            if (ic == nc - 1) {
                fprintf(stream, " %c\n", ch);
            } else {
                fprintf(stream, " %c", ch);
            }
        }
    }
}


void print_usage(FILE * stream, char * argv[]) {
    fprintf(stream, "Usage: %s FILENAME\n   Read IDX formatted data from FILENAME and ...\n", argv[0]);
}


float * scale_then_translate(Meta * meta, uint8_t * raw) {
    uint8_t lower = UINT8_MAX;
    uint8_t upper = 0;
    for (size_t i = 0; i < meta->n; i++) {
        if (raw[i] < lower) {
            lower = raw[i];
        }
        if (raw[i] > upper) {
            upper = raw[i];
        }
    }
    uint8_t range = upper - lower;

    float * data = calloc(meta->n, sizeof(float));
    for (size_t i = 0; i < meta->n; i++) {
        float x = raw[i] - lower;
        data[i] = x / range;
    }

    // set static variables to facilitate unscaling and
    // untranslating
    translation = lower;
    scale = range;
    return data;
}
