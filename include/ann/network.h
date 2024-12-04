#ifndef ANN__NETWORK_H
#define ANN__NETWORK_H
#include "idxread/idxread.h"
#include <stddef.h>
#include <stdio.h>


typedef struct network {
    float * biases;
    float * nodes;
    float * weights;
    size_t * nnodes;
    size_t nb;        // network total number of bias values
    size_t ni;        // network total number of input nodes
    size_t nl;        // network total number of layers
    size_t nn;        // network total number of nodes
    size_t no;        // network total number of output nodes
    size_t nw;        // network total number of weight values
} Network;

Network * ann__network_create (size_t, size_t []);
void ann__network_destroy (Network *);
void ann__network_fwd (Network *);
void ann__network_populate_biases (Network *);
void ann__network_populate_input (Network *, Meta *, float *, size_t);
void ann__network_populate_weights (Network *);
void ann__network_print(FILE *, Network *);

#endif
