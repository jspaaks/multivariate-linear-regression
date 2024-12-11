#ifndef LFUNS_H
#define LFUNS_H
#include <stddef.h>

float lfuns_softmax(const size_t n, const float * values, const size_t m);
float lfuns_svm(const size_t n, const float * values, const size_t m);

#endif
