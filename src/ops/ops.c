#include "afuns/afuns.h"
#include "data/data.h"
#include "ops/ops.h"
#include <assert.h>
#include <math.h>
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


void ops_map_af (ActivationFunction af, Matrix * in, Matrix * out) {
    assert(in->nr == out->nr && "Number of rows in input should match number of rows in output");
    assert(in->nc == out->nc && "Number of columns in input should match number of columns in output");
    size_t n = in->nr * in->nc;
    for (size_t i = 0; i < n; i++) {
        out->vals[i] = af(in->vals[i]);
    }
}

bool ops_map_eq (Matrix * a, Matrix * b, float eps) {
    assert(a->nr == b->nr && "Number of rows in 'a' should match number of rows in 'b'");
    assert(a->nc == b->nc && "Number of columns in 'a' should match number of columns in 'b'");
    size_t n = a->nr * a->nc;
    for (size_t i = 0; i < n; i++) {
        float ai = a->vals[i];
        float bi = b->vals[i];
        bool cond = fabsf(ai - bi) > eps;
        if (cond) {
            return false;
        }
    }
    return true;
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
