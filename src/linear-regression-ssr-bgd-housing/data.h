#ifndef DATA_H
#define DATA_H
#include "matrix/matrix.h"

void populate_features (const char * path, Matrix * features);
void populate_labels (const char * path, Matrix * labels);

#endif
