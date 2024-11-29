#include "idxread/idxread.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef uint8_t IdxDataType;

enum IdxDataType {
    IDX_DATA_TYPE_UINT8 = 0x08,
    IDX_DATA_TYPE_INT8 = 0x09,
    IDX_DATA_TYPE_INT16 = 0x0B,
    IDX_DATA_TYPE_INT32 = 0x0C,
    IDX_DATA_TYPE_FLOAT32 = 0x0D,
    IDX_DATA_TYPE_DOUBLE64 = 0x0E
};

void read_type (FILE *, IdxDataType *);
void read_ndimensions (FILE *, uint8_t *, size_t *);
void read_dimension_sizes (FILE *, uint8_t, uint32_t [256], size_t *);

bool idxread__read_uint8 (char * filename) {

    FILE * fp = fopen(filename, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "Error reading binary data from file '%s', aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    IdxDataType typ;
    uint8_t ndims;
    uint32_t dimension_sizes[256] = {0};
    size_t start;
    size_t stride;

    read_type(fp, &typ);
    read_ndimensions(fp, &ndims, &start);
    read_dimension_sizes(fp, ndims, &dimension_sizes[0], &stride);



    fclose(fp);

    fprintf(stdout, "ndims = %hhu\n", ndims);

    return true;
}


void read_type (FILE * fp, IdxDataType * typ) {
    fseek(fp, 2, SEEK_SET);
    fread(typ, 1, 1, fp);
}


void read_ndimensions (FILE * fp, uint8_t * ndims, size_t * start) {
    fseek(fp, 3, SEEK_SET);
    fread(ndims, 1, 1, fp);
    *start = 4 + *ndims * 4;
}

void read_dimension_sizes (FILE * fp, uint8_t ndims, uint32_t dimension_sizes[256], size_t * stride) {
    const uint8_t offset = 4;
    for (uint8_t idim = 0; idim < ndims; idim++) {
        uint32_t ell = 0;
        for (uint8_t i = 0; i < 4; i++) {
            fseek(fp, offset + idim * 4 + i, SEEK_SET);
            uint8_t part = 0;
            fread(&part, 1, 1, fp);
            uint8_t shift = (3 - i) * 8;
            ell = ell | part << shift;
        }
        dimension_sizes[idim] = ell;
    }

    if (ndims == 1) {
        *stride = 1;
    } else {
        *stride = dimension_sizes[1];
        for (uint8_t idim = 2; idim < ndims; idim++) {
            *stride *= dimension_sizes[idim];
        }
    }
}
