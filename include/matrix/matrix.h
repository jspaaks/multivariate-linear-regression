#ifndef MATRIX_H
#define MATRIX_H
#include "afuns/afuns.h"
#include <stddef.h>

typedef struct matrix Matrix;

struct matrix {
    size_t nr;
    size_t nc;
    float * vals;
};


void matrix_acc_cols (const Matrix * matrix, Matrix * result);
void matrix_acc_rows (const Matrix * matrix, Matrix * result);
void matrix_add (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_add_ (Matrix * left, const Matrix * right);
void matrix_concat_cols (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_concat_rows (const Matrix * top, const Matrix * bottom, Matrix * result);
Matrix * matrix_create (size_t nr, size_t nc);
void matrix_destroy (Matrix ** arr);
void matrix_dotproduct (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_map_af (const ActivationFunction af, const Matrix * in, Matrix * out);
void matrix_map_af_ (const ActivationFunction af, Matrix * matrix);
bool matrix_map_eq (const Matrix * a, const Matrix * b, float eps);
void matrix_scale (const Matrix * matrix, float factor, Matrix * result);
void matrix_scale_ (const Matrix * matrix, float factor);
void matrix_subtract (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_subtract_(Matrix * left, const Matrix * right);
void matrix_transpose (Matrix * input, const Matrix * transposed);

#endif
