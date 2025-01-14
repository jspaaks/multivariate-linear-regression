#include "data.h"
#include "matrix/matrix.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint32_t get_dimension_size (FILE * fp, const size_t idim);


Matrix * data_read_images (const char * path) {
    errno = 0;
    FILE * fp = fopen(path, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading binary data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    size_t nc = get_dimension_size(fp, 0);
    size_t nr = get_dimension_size(fp, 1) * get_dimension_size(fp, 2);
    size_t start = 4 * 1 + 3 * 4;
    Matrix * images = matrix_create (nr, nc);
    fseek(fp, start, SEEK_SET);
    for (size_t ic = 0; ic < nc; ic++) {
        for (size_t ir = 0; ir < nr; ir++) {
            uint8_t byte;
            fread(&byte, 1, 1, fp);
            size_t i = ir * nc + ic;
            images->xs[i] = (float) byte;
        }
    }
    fclose(fp);
    return images;
}


Matrix * data_read_labels (const char * path) {
    errno = 0;
    FILE * fp = fopen(path, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading binary data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    size_t nc = get_dimension_size(fp, 0);
    size_t nr = 1;
    size_t n = nr * nc;
    size_t start = 4 * 1 + 1 * 4;
    Matrix * labels = matrix_create(nr, nc);
    fseek(fp, start, SEEK_SET);
    for (size_t i = 0; i < n; i++) {
        uint8_t byte;
        fread(&byte, 1, 1, fp);
        labels->xs[i] = (float) byte;
    }
    fclose(fp);
    return labels;
}


uint32_t get_dimension_size (FILE * fp, const size_t idim) {
    const uint8_t offset = 4;
    const uint8_t nbytes = 4;
    uint32_t ell = 0;
    for (uint8_t i = 0; i < nbytes; i++) {
        fseek(fp, offset + idim * 4 + i, SEEK_SET);
        uint8_t part = 0;
        fread(&part, 1, 1, fp);
        uint8_t shift = (3 - i) * 8;
        ell |= part << shift;
    }
    return ell;
}
