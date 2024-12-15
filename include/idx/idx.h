#ifndef IDX_H
#define IDX_H
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
    float * vals;
};

void idx_destroy_data(Data ** data);
Data * idx_read (const char * path);
void idx_print_meta (FILE * fp, Data * data);


#endif
