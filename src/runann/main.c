#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "network/network.h"
#include "idx/idx.h"


void seed_network (Network *);
void print_usage (FILE *, char * []);
void print_image (FILE *, const Data *, size_t);


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

    const char * images_path = argv[1];
    const char * labels_path = argv[2];

    Data * images = (Data *) idx_read(images_path);
    Data * labels = (Data *) idx_read(labels_path);
    idx_print_meta(stdout, images);
    idx_print_meta(stdout, labels);

    // ============================================================ //

    constexpr size_t nlayers = 4;
    size_t nnodes[nlayers] = {784, 300, 100, 10};
    srand(time(nullptr));
    Network * network = network_create(nlayers, nnodes);
    network_print(stdout, network);
    float * losses = calloc(network->no, sizeof(float));

    const size_t nstarts = 1;
    const size_t niters = 1;
    const float learning_rate = 0.01;
    for (size_t istart = 0; istart < nstarts; istart++) {
        for (size_t iiter = 0; iiter < niters; iiter++) {
            network_populate_weights(network);
            network_populate_biases(network);
            for (size_t iobj = 0; iobj < images->nobjs; iobj++) {
                network_populate_input(network, images, iobj);
                network_fwdpass(network);
                network_calc_loss(network, labels, iobj, losses);
            }
            network_backprop(network, learning_rate);
        }
    }

    network_destroy(&network);
    idx_destroy_data(&images);
    idx_destroy_data(&labels);
    free(losses);
    losses = nullptr;

    return 0;
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
    fprintf(stream, "Usage: %s FILENAME\n   Read IDX formatted data from FILENAME and ...\n", argv[0]);
}
