#include "matrix/matrix.h"
#include "afuns/afuns.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void matrix_acc_cols(const Matrix * matrix, Matrix * result) {
}


void matrix_acc_rows(const Matrix * matrix, Matrix * result) {
}


void matrix_add (const Matrix * left, const Matrix * right, Matrix * result) {
    assert(left->nr == right->nr && "Expected number of rows to be equal when adding 2 instances of Matrix");
    assert(left->nc == right->nc && "Expected number of columns to be equal when adding 2 instances of Matrix");
    size_t n = left->nr * left->nc;
    for (size_t i = 0; i < n; i++) {
        result->vals[i] = left->vals[i] + right->vals[i];
    }
}


void matrix_add_ (Matrix * left, const Matrix * right) {
    assert(left->nr == right->nr && "Expected number of rows to be equal when adding 2 instances of Matrix");
    assert(left->nc == right->nc && "Expected number of columns to be equal when adding 2 instances of Matrix");
    size_t n = left->nr * left->nc;
    for (size_t i = 0; i < n; i++) {
        left->vals[i] = left->vals[i] + right->vals[i];
    }
}


void matrix_concat_cols(const Matrix * left, const Matrix * right, Matrix * result) {
}


void matrix_concat_rows(const Matrix * top, const Matrix * bottom, Matrix * result) {
}


Matrix * matrix_create (size_t nr, size_t nc) {
    errno = 0;
    Matrix * arr = calloc(1, sizeof(Matrix));
    if (arr == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for Matrix instance, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }

    errno = 0;
    float * vals = calloc(nr * nc, sizeof(float));
    if (vals == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for values of Matrix instance, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }

    arr->nr = nr;
    arr->nc = nc;
    arr->n = nr * nc;
    arr->vals = vals;
    return arr;
}


void matrix_destroy(Matrix ** arr) {
    free((*arr)->vals);
    (*arr)->vals = nullptr;
    free(*arr);
    *arr = nullptr;
}


void matrix_dotproduct (const Matrix * left, const Matrix * right, Matrix * result) {
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


void matrix_map_af (const ActivationFunction af, const Matrix * in, Matrix * out) {
    assert(in->nr == out->nr && "Number of rows in input should match number of rows in output");
    assert(in->nc == out->nc && "Number of columns in input should match number of columns in output");
    for (size_t i = 0; i < in->n; i++) {
        out->vals[i] = af(in->vals[i]);
    }
}

void matrix_map_af_ (const ActivationFunction af, Matrix * matrix) {
}


bool matrix_map_eq (const Matrix * a, const Matrix * b, float eps) {
    assert(a->nr == b->nr && "Number of rows in 'a' should match number of rows in 'b'");
    assert(a->nc == b->nc && "Number of columns in 'a' should match number of columns in 'b'");
    for (size_t i = 0; i < a->n; i++) {
        float ai = a->vals[i];
        float bi = b->vals[i];
        bool cond = fabsf(ai - bi) > eps;
        if (cond) {
            return false;
        }
    }
    return true;
}


void matrix_scale (const Matrix * matrix, float factor, Matrix * result) {
    for (size_t i = 0; i < matrix->n; i++) {
        result->vals[i] = matrix->vals[i] * factor;
    }
}


void matrix_scale_ (const Matrix * matrix, float factor) {
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = matrix->vals[i] * factor;
    }
}


void matrix_subtract (const Matrix * left, const Matrix * right, Matrix * result) {
    assert(left->nr == right->nr && "Expected number of rows to be equal when subtracting 2 instances of Matrix");
    assert(left->nc == right->nc && "Expected number of columns to be equal when subtracting 2 instances of Matrix");
    for (size_t i = 0; i < left->n; i++) {
        result->vals[i] = left->vals[i] - right->vals[i];
    }
}


void matrix_subtract_ (Matrix * left, const Matrix * right) {
    assert(left->nr == right->nr && "Expected number of rows to be equal when subtracting 2 instances of Matrix");
    assert(left->nc == right->nc && "Expected number of columns to be equal when subtracting 2 instances of Matrix");
    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = left->vals[i] - right->vals[i];
    }
}


void matrix_transpose (Matrix * input, const Matrix * transposed) {
    assert(input->nr == transposed->nc && "Number of rows in input should be equal to the number of columns in the output.");
    assert(input->nc == transposed->nr && "Number of columns in input should be equal to the number of rows in the output.");
    size_t nr = input->nr;
    size_t nc = input->nc;
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < nc; ic++) {
            size_t i = ir * nc + ic;
            size_t j = ic * nr + ir;
            transposed->vals[j] = input->vals[i];
        }
    }
}