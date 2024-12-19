#ifndef MATRIX_H
#define MATRIX_H
#include <stddef.h>
#include <stdio.h>

typedef struct matrix Matrix;

struct matrix {
    size_t nr;
    size_t nc;
    size_t n;
    float * vals;
};

void matrix_acc_cols (const Matrix * matrix, Matrix * result);
void matrix_acc_rows (const Matrix * matrix, Matrix * result);
void matrix_add (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_add_ (Matrix * left, const Matrix * right);
void matrix_add_scalar (const Matrix * left, float right, Matrix * result);
void matrix_add_scalar_ (Matrix * left, float right);
float matrix_avg (const Matrix * matrix);
void matrix_avg_per_col (const Matrix * matrix, Matrix * result);
void matrix_avg_per_row (const Matrix * matrix, Matrix * result);
void matrix_broadcast_down (const Matrix * matrix, Matrix * result);
void matrix_broadcast_right (const Matrix * matrix, Matrix * result);
Matrix * matrix_create (size_t nr, size_t nc);
void matrix_destroy (Matrix ** arr);
void matrix_divide_scalar (const Matrix * matrix, float factor, Matrix * result);
void matrix_divide_scalar_ (const Matrix * matrix, float factor);
void matrix_dotproduct (const Matrix * left, const Matrix * right, Matrix * result);
float matrix_highest (const Matrix * matrix);
void matrix_highest_per_col (const Matrix * matrix, Matrix * result);
void matrix_highest_per_row (const Matrix * matrix, Matrix * result);
float matrix_lowest (const Matrix * matrix);
void matrix_lowest_per_col (const Matrix * matrix, Matrix * result);
void matrix_lowest_per_row (const Matrix * matrix, Matrix * result);
bool matrix_map_eq (const Matrix * a, const Matrix * b, float eps);
void matrix_multiply_scalar (const Matrix * matrix, float factor, Matrix * result);
void matrix_multiply_scalar_ (const Matrix * matrix, float factor);
void matrix_print (FILE * stream, Matrix * matrix, char * title);
void matrix_subtract (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_subtract_ (Matrix * left, const Matrix * right);
void matrix_subtract_scalar (const Matrix * left, float right, Matrix * result);
void matrix_subtract_scalar_ (Matrix * left, float right);
void matrix_transpose (Matrix * input, const Matrix * transposed);
void matrix_zero_ (Matrix * matrix);

#endif
