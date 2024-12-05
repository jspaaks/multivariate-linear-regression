#ifndef IDXREAD_H
#define IDXREAD_H
#include <stdio.h>
#include <stdint.h>


#define IDX_DATA_TYPE_UINT8 0x8

typedef struct data Data;

struct data {
    uint32_t dimension_sizes[256];
    uint8_t ndims;
    size_t nobjs;
    size_t start;
    size_t stride;
    size_t n;
    uint8_t typ;
    uint8_t * vals;
};

void idx__destroy_data(Data ** data);
Data * idx__read (const char * path);
void idx__print_meta (FILE * fp, Data * data);


#endif
