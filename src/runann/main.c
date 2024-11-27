#include <stdio.h>
#include "ann/network.h"

int main (void) {

    constexpr size_t nlayers = 3;
    size_t nnodes[nlayers] = {3, 4, 2};

    Network * network = ann__network_create(nlayers, nnodes);

    fprintf(stdout, "network total number of\n");
    fprintf(stdout, "- nodes: %zu\n", network->nn);
    fprintf(stdout, "- layers: %zu\n", network->nl);
    fprintf(stdout, "- weights: %zu\n", network->nw);
    fprintf(stdout, "- biases: %zu\n", network->nb);

    ann__network_fwd(network);

    ann__network_destroy(network);

    return 0;
}
