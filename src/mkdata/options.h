#ifndef OPTIONS_H
#define OPTIONS_H
#include "matrix/matrix.h"


void scan_for_basename (int argc, char * argv[], char * basename);
bool scan_for_help (int argc, char * argv[]);
size_t scan_for_nfeatures (int argc, char * argv[]);
size_t scan_for_nsamples (int argc, char * argv[]);
float scan_for_sigma (int argc, char * argv[]);
void scan_for_lower_bounds (int argc, char * argv[], size_t nfeatures, Matrix * lower_bounds);
void scan_for_true_weights (int argc, char * argv[], size_t nfeatures, Matrix * features);
bool scan_for_unauthorized (int argc, char * argv[]);
void scan_for_upper_bounds (int argc, char * argv[], size_t nfeatures, Matrix * upper_bounds);

#endif
