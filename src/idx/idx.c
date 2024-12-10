#include "idx/idx.h"
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void fill_dimension_sizes (FILE *, Data *);
void fill_n (Data *);
void fill_ndims (FILE *, Data *);
void fill_nobjs (Data *);
void fill_start(Data *);
void fill_stride (Data *);
void fill_type (FILE *, Data *);


void fill_dimension_sizes (FILE * fp, Data * data) {
    const uint8_t offset = 4;
    for (uint8_t idim = 0; idim < data->ndims; idim++) {
        uint32_t ell = 0;
        for (uint8_t i = 0; i < 4; i++) {
            fseek(fp, offset + idim * 4 + i, SEEK_SET);
            uint8_t part = 0;
            fread(&part, 1, 1, fp);
            uint8_t shift = (3 - i) * 8;
            ell = ell | part << shift;
        }
        data->dimension_sizes[idim] = ell;
    }
}


void fill_n (Data * data) {
    data->n = data->nobjs * data->stride;
}


void fill_ndims (FILE * fp, Data * data) {
    fseek(fp, 3, SEEK_SET);
    fread(&data->ndims, 1, 1, fp);
}


void fill_nobjs (Data * data) {
    data->nobjs = data->dimension_sizes[0];
}


void fill_start (Data * data) {
    data->start = 4 + data->ndims * 4;
}


void fill_stride (Data * data) {
    data->stride = 1;
    if (data->ndims > 1) {
        data->stride = data->dimension_sizes[1];
        for (uint8_t idim = 2; idim < data->ndims; idim++) {
            data->stride *= data->dimension_sizes[idim];
        }
    }
}


void fill_type (FILE * fp, Data * data) {
    fseek(fp, 2, SEEK_SET);
    fread(&data->typ, 1, 1, fp);
}


void idx_destroy_data(Data ** data) {
    free((*data)->vals);
    free(*data);
    *data = nullptr;
}


Data * idx_read (const char * path) {

    errno = 0;
    Data * data = calloc(1, sizeof(Data));
    if (data == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for data, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }

    FILE * fp = fopen(path, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading binary data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }

    fill_type(fp, data);
    fill_ndims(fp, data);
    fill_dimension_sizes(fp, data);
    fill_start(data);
    fill_nobjs(data);
    fill_stride(data);
    fill_n(data);

    assert(data->typ == IDX_DATA_TYPE_UINT8 && "Only uint8_t are supported.");

    const uint8_t nbytes_per_elem = 1;

    data->vals = calloc(data->nobjs, data->stride * nbytes_per_elem);
    if (data->vals == nullptr) {
        fprintf(stderr, "Error allocating memory for values, aborting.\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp, data->start, SEEK_SET);
    fread(data->vals, nbytes_per_elem, data->nobjs * data->stride, fp);
    fclose(fp);
    return data;
}



void idx_print_meta (FILE * stream, Data * data) {
    fprintf(stream, "{\n");
    fprintf(stream, "  .dimension_sizes = {");
    for (uint8_t i = 0; i < data->ndims - 1; i++) {
        fprintf(stream, "%u,", data->dimension_sizes[i]);
    }
    fprintf(stream, "%u}\n", data->dimension_sizes[data->ndims - 1]);
    fprintf(stream, "  .n               = %zu\n", data->n);
    fprintf(stream, "  .ndims           = %hhu\n", data->ndims);
    fprintf(stream, "  .nobjs           = %zu\n", data->nobjs);
    fprintf(stream, "  .start           = %zu bytes\n", data->start);
    fprintf(stream, "  .stride          = %zu bytes\n", data->stride);
    fprintf(stream, "  .typ             = %hhu\n", data->typ);
    fprintf(stream, "}\n");
}
