#ifndef OPTIONS_H
#define OPTIONS_H
#include "kwargs/kwargs.h"
#include "matrix/matrix.h"
#include <stddef.h>


const char * get_basename (const Kwargs * kwargs);
const KwargsClass * get_classes (void);
size_t get_nclasses (void);
size_t get_nfeatures (const Kwargs * kwargs);
void get_lower_bounds (const Kwargs * kwargs, Matrix * true_weights, size_t nfeatures);
size_t get_nsamples (const Kwargs * kwargs);
float get_sigma (const Kwargs * kwargs);
void get_true_weights (const Kwargs * kwargs, Matrix * true_weights, size_t nfeatures);
void get_upper_bounds (const Kwargs * kwargs, Matrix * true_weights, size_t nfeatures);

#endif
