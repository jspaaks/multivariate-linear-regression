#ifndef RUN_H
#define RUN_H
#include <stddef.h>

void run (size_t nsamples, size_t nfeatures, size_t nepochs, float learning_rate,
          bool standardize, bool verbose, const char * features_path, const char * labels_path);

#endif
