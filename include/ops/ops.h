#ifndef OPS_H
#define OPS_H
#include "matrix/matrix.h"
#include <stddef.h>

void ops_add_bias (void);
float ops_half_ssr(const Matrix * predicted, const Matrix * labels);
void ops_softmax (void);
void ops_svm(Matrix * scores, Matrix * labels, float * loss);

#endif
