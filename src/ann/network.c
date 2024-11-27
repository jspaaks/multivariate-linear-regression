#include "ann/network.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"


size_t calc_nb (size_t, size_t *);
size_t calc_nn (size_t, size_t *);
size_t calc_nw (size_t, size_t *);


Network * ann__network_create (size_t nl, size_t * nnodes) {
    Network * network = malloc(1 * sizeof (Network));
    if (network == nullptr) {
        fprintf(stderr, "Something went wrong allocating memory for 'Network'. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    network->nnodes = nnodes;
    network->nl = nl;
    network->nn = calc_nn(nl, nnodes);
    network->nw = calc_nw(nl, nnodes);
    network->nb = calc_nb(nl, nnodes);
    network->nodes = malloc(network->nn * sizeof(float));
    if (network->nodes == nullptr) {
        fprintf(stderr, "Something went wrong allocating memory for 'Network' member 'nodes'. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    network->biases = malloc(network->nb * sizeof(float));
    if (network->biases == nullptr) {
        fprintf(stderr, "Something went wrong allocating memory for 'Network' member 'biases'. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    network->weights = malloc(network->nw * sizeof(float));
    if (network->weights == nullptr) {
        fprintf(stderr, "Something went wrong allocating memory for 'Network' member 'weights'. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    return network;
}


void ann__network_fwd (Network * network) {
    size_t nl = network->nl;
    float * w = &network->weights[0];
    float * in = &network->nodes[0];
    float * out = in + network->nnodes[0];
    for (size_t il = 0; il < nl - 1; il++) {
        size_t inr = network->nnodes[il];
        size_t onr = network->nnodes[il+1];
        for (size_t i = 0; i < onr; i++) {
            *out = 0;
            for (size_t j = 0; j < inr; j++) {
                *out += *w * *in;
                w++;
                in++;
            }
            out++;
            if (i < onr - 1) {
                in -= inr;
            }
        }
    }
}


void ann__network_destroy (Network * network) {
    free(network->nodes);
    network->nodes = nullptr;
    free(network->biases);
    network->biases = nullptr;
    free(network->weights);
    network->weights = nullptr;
    free(network);
    network = nullptr;
}


size_t calc_nb (size_t nl, size_t * nnodes) {
    size_t n = 0;
    for (size_t i = 1; i < nl; i++) {
        n += nnodes[i];
    }
    return n;
}


size_t calc_nn (size_t nl, size_t * nnodes) {
    size_t n = 0;
    for (size_t i = 0; i < nl; i++) {
        n += nnodes[i];
    }
    return n;
}

size_t calc_nw (size_t nl, size_t * nnodes) {
    size_t n = 0;
    for (size_t i = 0; i < nl - 1; i++) {
        n += nnodes[i] * nnodes[i+1];
    }
    return n;
}
