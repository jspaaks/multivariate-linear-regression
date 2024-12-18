#ifndef OPS_H
#define OPS_H
#include "afuns/afuns.h"
#include "data/data.h"
#include "ops/ops.h"
#include <stddef.h>

void ops_add_bias (void);
float ops_half_ssr(const Matrix * predicted, const Matrix * labels);
void ops_softmax (void);
void ops_svm(Matrix * scores, Matrix * labels, float * loss);

#endif
