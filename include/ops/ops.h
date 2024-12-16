#ifndef OPS_H
#define OPS_H
#include "data/data.h"
#include "ops/ops.h"
#include <stddef.h>

void ops_add_bias (void);
void ops_dot_product (Matrix * left, Matrix * right, Matrix * result);
void ops_softmax (void);
void ops_svm(Matrix * scores, Matrix * labels, float * loss);


#endif
