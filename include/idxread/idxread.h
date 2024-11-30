#ifndef IDXREAD_H
#define IDXREAD_H
#include <stdio.h>
#include <stdint.h>


typedef uint8_t IdxDataType;
typedef struct meta Meta;

enum IdxDataType {
    IDX_DATA_TYPE_UINT8 = 0x08,
    IDX_DATA_TYPE_INT8 = 0x09,
    IDX_DATA_TYPE_INT16 = 0x0B,
    IDX_DATA_TYPE_INT32 = 0x0C,
    IDX_DATA_TYPE_FLOAT32 = 0x0D,
    IDX_DATA_TYPE_DOUBLE64 = 0x0E
};

struct meta {
    uint32_t dimension_sizes[256];
    uint8_t ndims;
    size_t nobjs;
    size_t start;
    size_t stride;
    IdxDataType typ;
};

uint8_t * idxread__get_data (const char *, Meta *);
void idxread__get_meta (const char *, Meta *);
void idxread__print_meta (FILE *, Meta *);

#endif
