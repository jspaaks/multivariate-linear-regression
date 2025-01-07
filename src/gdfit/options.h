#ifndef OPTIONS_H
#define OPTIONS_H
#include "kwargs/kwargs.h"
#include "matrix/matrix.h"
#include <stdlib.h>


const char * get_basename (const Kwargs * kwargs);
float get_learning_rate (const Kwargs * kwargs);
size_t get_nepochs (const Kwargs * kwargs);


#endif
