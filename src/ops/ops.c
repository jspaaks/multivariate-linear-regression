#include "afuns/afuns.h"
#include "matrix/matrix.h"
#include "ops/ops.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void ops_add_bias (void) {
}


float ops_half_ssr(const Matrix * predicted, const Matrix * labels) {
    assert(predicted->nr == labels->nr && "expected equal number of rows");
    assert(predicted->nc == labels->nc && "expected equal number of rows");
    assert(predicted->nr == 1 && "expected predicted number of rows to be 1");
    assert(labels->nr == 1 && "expected labels number of rows to be 1");
    size_t n = labels->nc;
    float rv = 0.0f;
    for (size_t i = 0; i < n; i++) {
        float d = predicted->vals[i] - labels->vals[i];
        rv += pow(d, 2);
    }
    // divide by 2 because that simplifies the derivative without affecting
    // our ability to minimize the loss
    return rv / 2;
}


void ops_softmax (void) {
}


void ops_svm(Matrix * scores, Matrix * labels, float * loss) {
    Matrix * losses = matrix_create(1, scores->nc);
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
    matrix_destroy(&losses);
}
