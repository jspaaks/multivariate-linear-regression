#include "data/data.h"
#include "ops/ops.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void ops_add_bias (void) {
}


void ops_dot_product (Matrix * left, Matrix * right, Matrix * result) {
    assert(left->nc == right->nr && "Number of columns in left operand should be equal to the number of rows in the right operand.");
    assert(result->nr == left->nr && "Number of rows in result should match the number of rows in left operand.");
    assert(result->nc == right->nc && "Number of columns in result should match the number of columns in right operand.");
    size_t nr = result->nr;
    size_t nc = result->nc;
    size_t n = left->nc;
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < nc; ic++) {
            size_t iresult = ir * nc + ic;
            result->vals[iresult] = 0.0f;
            for (size_t i = 0; i < n; i++) {
                size_t ileft = ir * left->nc + i;
                size_t iright = ic + i * right->nc;
                result->vals[iresult] += left->vals[ileft] * right->vals[iright];
            }
        }
    }
}


void ops_softmax (void) {
}


void ops_svm(Matrix * scores, Matrix * labels, float * loss) {
    Matrix * losses = data_create(1, scores->nc);
    for (size_t ic = 0; ic < scores->nc; ic++) {
        float acc = 0.0;
        size_t m = (size_t) labels->vals[ic];
        for (size_t ir = 0; ir < scores->nr; ir++) {
            size_t i = ir * scores->nc + ic;
            size_t j = m * scores->nc + ic;
            float tmp = scores->vals[i] - scores->vals[j] + 1;
            acc += tmp > 0 ? tmp : 0;
        }
        losses->vals[ic] = acc - 1.0f;
    }
    *loss = 0.0f;
    for (size_t i = 0; i < losses->nc; i++) {
        *loss += losses->vals[i] / losses->nc;
    }
    data_destroy(&losses);
}
