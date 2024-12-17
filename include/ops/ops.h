#ifndef OPS_H
#define OPS_H
#include "afuns/afuns.h"
#include "data/data.h"
#include "ops/ops.h"
#include <stddef.h>

void ops_add_bias (void);
void ops_dot_product (Matrix * left, Matrix * right, Matrix * result);
void ops_map_af (ActivationFunction af, Matrix * in, Matrix * out);
bool ops_map_eq (Matrix * a, Matrix * b, float eps);
void ops_softmax (void);
void ops_svm(Matrix * scores, Matrix * labels, float * loss);


#endif
