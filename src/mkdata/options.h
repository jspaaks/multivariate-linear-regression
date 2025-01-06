#ifndef OPTIONS_H
#define OPTIONS_H
#include "kwargs/kwargs.h"
#include "matrix/matrix.h"
#include <stddef.h>


const char * get_basename (int argc, const char * argv[],
                           size_t nclasses, const KwargsClass * classes,
                           size_t nclassifieds, const KwargsType * classifieds);

const KwargsClass * get_classes (void);

size_t get_nclasses (void);

size_t get_nfeatures (int argc, const char * argv[],
                      size_t nclasses, const KwargsClass * classes,
                      size_t nclassifieds, const KwargsType * classifieds);

void get_lower_bounds (int argc, const char * argv[],
                       size_t nclasses, const KwargsClass * classes,
                       size_t nclassifieds, const KwargsType * classifieds,
                       Matrix * true_weights, size_t nfeatures);

size_t get_nsamples (int argc, const char * argv[],
                     size_t nclasses, const KwargsClass * classes,
                     size_t nclassifieds, const KwargsType * classifieds);

float get_sigma (int argc, const char * argv[],
                 size_t nclasses, const KwargsClass * classes,
                 size_t nclassifieds, const KwargsType * classifieds);

void get_true_weights (int argc, const char * argv[],
                       size_t nclasses, const KwargsClass * classes,
                       size_t nclassifieds, const KwargsType * classifieds,
                       Matrix * true_weights, size_t nfeatures);

void get_upper_bounds (int argc, const char * argv[],
                       size_t nclasses, const KwargsClass * classes,
                       size_t nclassifieds, const KwargsType * classifieds,
                       Matrix * true_weights, size_t nfeatures);

#endif
