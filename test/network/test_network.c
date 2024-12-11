#include <criterion/criterion.h>
#include "network/network.h"

Test(network, create_and_destroy){

    constexpr size_t nl = 4;
    size_t nnodes[nl] = {5,2,3,6};
    Network * network = network_create (nl, &nnodes[0]);

    {
        const char msg[] = "problem with network total number of";
        const Network expected = {
            .nb = 11,
            .ni = 5,
            .nl = 4,
            .nn = 16,
            .no = 6,
            .nw = 34
        };

        cr_assert(network->nb == expected.nb, "%s bias values (expected: %zu, actual: %zu)", msg, expected.nb, network->nb);
        cr_assert(network->ni == expected.ni, "%s input nodes (expected: %zu, actual: %zu)", msg, expected.ni, network->ni);
        cr_assert(network->nl == expected.nl, "%s layers (expected: %zu, actual: %zu)", msg, expected.nl, network->nl);
        cr_assert(network->nn == expected.nn, "%s nodes (expected: %zu, actual: %zu)", msg, expected.nn, network->nn);
        cr_assert(network->no == expected.no, "%s output nodes (expected: %zu, actual: %zu)", msg, expected.no, network->no);
        cr_assert(network->nw == expected.nw, "%s weight values (expected: %zu, actual: %zu)", msg, expected.nw, network->nw);
    }

    cr_assert(network->nnodes != nullptr, "instance of Network should have an initialized member 'nnodes'");
    cr_assert(network->biases != nullptr, "instance of Network should have an initialized member 'biases'");
    cr_assert(network->weights != nullptr, "instance of Network should have an initialized member 'weights'");
    cr_assert(network->nodes != nullptr, "instance of Network should have an initialized member 'nodes'");

    for (size_t i = 0; i < nl; i++) {
        size_t actual = network->nnodes[i];
        size_t expected = nnodes[i];
        cr_assert(actual == expected, "expected nnodes[%zu] to be %zu but found %zu", i, expected, actual);
    }

    for (size_t i = 0; i < network->nb; i++) {
        float bias = network->biases[i];
        cr_assert(bias == 0, "expected biases[%zu] to be zero but found %f", i, bias);
    }

    for (size_t i = 0; i < network->nw; i++) {
        float weight = network->weights[i];
        cr_assert(weight == 0, "expected weights[%zu] to be zero but found %f", i, weight);
    }

    for (size_t i = 0; i < network->nn; i++) {
        float node = network->nodes[i];
        cr_assert(node == 0, "expected nodes[%zu] to be zero but found %f", i, node);
    }

    network_destroy(&network);
    cr_assert(network == nullptr, "not sure if memory previously allocated for network has been freed correctly.");
}


Test(network, backprop){
    cr_skip("not implemented");
}

Test(network, calc_losses){
    cr_skip("not implemented");
}

Test(network, fwdpass){
    cr_skip("not implemented");
}

Test(network, populate_biases){
    cr_skip("not implemented");
}

Test(network, populate_input){
    cr_skip("not implemented");
}

Test(network, populate_weights){
    cr_skip("not implemented");
}

Test(network, print){
    cr_skip("not implemented");
}
