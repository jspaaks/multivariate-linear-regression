#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ann/network.h"
#include "idxread/idxread.h"


void seed_network (Network *);
void read_data (void);
void print_usage (FILE *, char * []);
void print_image (FILE *, const Meta *, const uint8_t *, size_t);

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
    uint8_t * data = idxread__get_data(path, &meta);
    idxread__print_meta(stdout, &meta);
    print_image(stdout, &meta, &data[0], 0);

    // ============================================================ //

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

    free(data);

    return 0;
}


void print_image (FILE * stream, const Meta * meta, const uint8_t * data, size_t iobj) {
    size_t nr = meta->dimension_sizes[1];
    size_t nc = meta->dimension_sizes[2];
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
