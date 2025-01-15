#ifndef OPTIONS_H
#define OPTIONS_H
#include "kwargs/kwargs.h"
#include "matrix/matrix.h"
#include <stddef.h>
#include <stdio.h>


const KwargsClass * options_get_classes (void);
const char * options_get_features_path (const Kwargs * kwargs);
void options_get_initial_weights (const Kwargs * kwargs, Matrix * weights);
const char * options_get_labels_path (const Kwargs * kwargs);
float options_get_learning_rate (const Kwargs * kwargs);
size_t options_get_nclasses (void);
size_t options_get_nepochs (const Kwargs * kwargs);
bool options_get_standardize (const Kwargs * kwargs);
bool options_get_verbose (const Kwargs * kwargs);
void options_show_usage (FILE * stream);


#endif
