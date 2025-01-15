#ifndef OPTIONS_H
#define OPTIONS_H
#include "kwargs/kwargs.h"
#include "matrix/matrix.h"
#include <stddef.h>


const char * options_get_basename (const Kwargs * kwargs);
const KwargsClass * options_get_classes (void);
size_t options_get_nclasses (void);
size_t options_get_nfeatures (const Kwargs * kwargs);
void options_get_lower_bounds (const Kwargs * kwargs, Matrix * true_weights, size_t nfeatures);
size_t options_get_nsamples (const Kwargs * kwargs);
float options_get_sigma (const Kwargs * kwargs);
void options_get_true_weights (const Kwargs * kwargs, Matrix * true_weights, size_t nfeatures);
void options_get_upper_bounds (const Kwargs * kwargs, Matrix * true_weights, size_t nfeatures);
void options_show_usage (FILE * stream);
bool options_get_verbose (const Kwargs * kwargs);

#endif
