#include "idxread/idxread.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void fill_meta_dimension_sizes (FILE *, Meta *);
void fill_meta_n (Meta *);
void fill_meta_ndims (FILE *, Meta *);
void fill_meta_nobjs (Meta *);
void fill_meta_start(Meta *);
void fill_meta_stride (Meta *);
void fill_meta_type (FILE *, Meta *);


void fill_meta_dimension_sizes (FILE * fp, Meta * meta) {
    const uint8_t offset = 4;
    for (uint8_t idim = 0; idim < meta->ndims; idim++) {
        uint32_t ell = 0;
        for (uint8_t i = 0; i < 4; i++) {
            fseek(fp, offset + idim * 4 + i, SEEK_SET);
            uint8_t part = 0;
            fread(&part, 1, 1, fp);
            uint8_t shift = (3 - i) * 8;
            ell = ell | part << shift;
        }
        meta->dimension_sizes[idim] = ell;
    }
}


void fill_meta_n (Meta * meta) {
    meta->n = meta->nobjs * meta->stride;
}


void fill_meta_ndims (FILE * fp, Meta * meta) {
    fseek(fp, 3, SEEK_SET);
    fread(&meta->ndims, 1, 1, fp);
}


void fill_meta_nobjs (Meta * meta) {
    meta->nobjs = meta->dimension_sizes[0];
}


void fill_meta_start (Meta * meta) {
    meta->start = 4 + meta->ndims * 4;
}


void fill_meta_stride (Meta * meta) {
    meta->stride = 1;
    if (meta->ndims > 1) {
        meta->stride = meta->dimension_sizes[1];
        for (uint8_t idim = 2; idim < meta->ndims; idim++) {
            meta->stride *= meta->dimension_sizes[idim];
        }
    }
}


void fill_meta_type (FILE * fp, Meta * meta) {
    fseek(fp, 2, SEEK_SET);
    fread(&meta->typ, 1, 1, fp);
}


uint8_t * idxread__get_data (const char * path, Meta * meta) {
    errno = 0;
    FILE * fp = fopen(path, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError opening binary file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    uint8_t nbytes_per_elem;
    if (meta->typ == IDX_DATA_TYPE_UINT8) {
        nbytes_per_elem = 1;
    } else {
        fprintf(stderr, "Not implemented\n");
    }
    uint8_t * p = calloc(meta->nobjs, meta->stride * nbytes_per_elem);
    if (p == nullptr) {
        fprintf(stderr, "Error allocating memory, aborting.\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp, meta->start, SEEK_SET);
    fread(p, nbytes_per_elem, meta->nobjs * meta->stride, fp);
    fclose(fp);
    return p;
}

void idxread__get_meta (const char * path, Meta * meta) {
    errno = 0;
    FILE * fp = fopen(path, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading binary data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    fill_meta_type(fp, meta);
    fill_meta_ndims(fp, meta);
    fill_meta_dimension_sizes(fp, meta);
    fclose(fp);
    fill_meta_start(meta);
    fill_meta_nobjs(meta);
    fill_meta_stride(meta);
    fill_meta_n(meta);
}


void idxread__print_meta (FILE * stream, Meta * meta) {
    fprintf(stream, "{\n");
    fprintf(stream, "  .dimension_sizes = {");
    for (uint8_t i = 0; i < meta->ndims - 1; i++) {
        fprintf(stream, "%u,", meta->dimension_sizes[i]);
    }
    fprintf(stream, "%u}\n", meta->dimension_sizes[meta->ndims - 1]);
    fprintf(stream, "  .n               = %zu\n", meta->n);
    fprintf(stream, "  .ndims           = %hhu\n", meta->ndims);
    fprintf(stream, "  .nobjs           = %zu\n", meta->nobjs);
    fprintf(stream, "  .start           = %zu bytes\n", meta->start);
    fprintf(stream, "  .stride          = %zu bytes\n", meta->stride);
    fprintf(stream, "  .typ             = %hhu\n", meta->typ);
    fprintf(stream, "}\n");
}
