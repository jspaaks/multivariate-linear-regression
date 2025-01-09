#ifndef OPTIONS_H
#define OPTIONS_H
#include "kwargs/kwargs.h"
#include "matrix/matrix.h"
#include <stdlib.h>


const char * get_basename (const Kwargs * kwargs);
const KwargsClass * get_classes (void);
float get_learning_rate (const Kwargs * kwargs);
size_t get_nepochs (const Kwargs * kwargs);
size_t get_nclasses (void);
void show_usage (FILE * stream);

#endif
