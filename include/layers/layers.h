#ifndef LAYERS_H
#define LAYERS_H
#include <stddef.h>

typedef struct dot_product_layer DotProductLayer;

struct dot_product_layer {
    void (*fwd) (const DotProductLayer *, const float *, float *);
    size_t ni;
    size_t no;
    float * weights;
    float * gradients;
};

DotProductLayer * layers_create_dot_product_layer (size_t ni, size_t no);
void layers_destroy_dot_product_layer (DotProductLayer ** dpl);

#endif
