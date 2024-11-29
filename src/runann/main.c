#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ann/network.h"
#include "idxread/idxread.h"


void seed_network (Network *);
void read_data (void);

int main (void) {

    read_data();

    constexpr size_t nlayers = 3;
    size_t nnodes[nlayers] = {3, 4, 2};

    srand(time(nullptr));

    Network * network = ann__network_create(nlayers, nnodes);
    seed_network(network);

    fprintf(stdout, "network total number of\n");
    fprintf(stdout, "- nodes: %zu\n", network->nn);
    fprintf(stdout, "- layers: %zu\n", network->nl);
    fprintf(stdout, "- weights: %zu\n", network->nw);
    fprintf(stdout, "- biases: %zu\n", network->nb);

    ann__network_fwd(network);

    ann__network_destroy(network);

    return 0;
}


void read_data(void) {
    char * filename = "t10k-images.idx3-ubyte";
    idxread__read_uint8 (filename);
}


void seed_network (Network * network) {
    size_t nb = network->nb;
    for (size_t i = 0; i < nb; i++) {
        int z = rand() % 100;
        network->biases[i] = (float) z / 100;
    }
    size_t nn = network->nn;
    for (size_t i = 0; i < nn; i++) {
        int z = rand() % 100;
        network->nodes[i] = (float) z / 100;
    }
    size_t nw = network->nw;
    for (size_t i = 0; i < nw; i++) {
        int z = rand() % 100;
        network->weights[i] = (float) z / 100;
    }
}
