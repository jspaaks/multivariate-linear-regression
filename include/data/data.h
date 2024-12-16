#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include <stdint.h>

typedef struct matrix Matrix;

struct matrix {
    size_t nr;
    size_t nc;
    float * vals;
};


Matrix * data_create (size_t nr, size_t nc);
void data_destroy(Matrix ** arr);
Matrix * data_read_images (const char * path);
Matrix * data_read_labels (const char * path);

#endif
