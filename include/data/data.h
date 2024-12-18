#ifndef DATA_H
#define DATA_H
#include "matrix/matrix.h"
#include <stdio.h>
#include <stdint.h>


Matrix * data_read_images (const char * path);
Matrix * data_read_labels (const char * path);

#endif
