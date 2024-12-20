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

void matrix_accd (const Matrix * matrix, Matrix * result);
void matrix_accr (const Matrix * matrix, Matrix * result);
void matrix_add (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_add_ (Matrix * left, const Matrix * right);
void matrix_adds (const Matrix * left, float right, Matrix * result);
void matrix_adds_ (Matrix * left, float right);
float matrix_avg (const Matrix * matrix);
void matrix_avgd (const Matrix * matrix, Matrix * result);
void matrix_avgr (const Matrix * matrix, Matrix * result);
void matrix_bcastd (const Matrix * matrix, Matrix * result);
void matrix_bcastr (const Matrix * matrix, Matrix * result);
Matrix * matrix_create (size_t nr, size_t nc);
void matrix_destroy (Matrix ** arr);
void matrix_divs (const Matrix * matrix, float factor, Matrix * result);
void matrix_divs_ (const Matrix * matrix, float factor);
void matrix_dotp (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_hadp (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_hadp_ (Matrix * left, const Matrix * right);
float matrix_max (const Matrix * matrix);
void matrix_maxd (const Matrix * matrix, Matrix * result);
void matrix_maxr (const Matrix * matrix, Matrix * result);
float matrix_min (const Matrix * matrix);
void matrix_mind (const Matrix * matrix, Matrix * result);
void matrix_minr (const Matrix * matrix, Matrix * result);
bool matrix_map_eq (const Matrix * a, const Matrix * b, float eps);
void matrix_print (FILE * stream, Matrix * matrix, char * title);
void matrix_scap (const Matrix * left, float right, Matrix * result);
void matrix_scap_ (Matrix * left, float right);
void matrix_sub (const Matrix * left, const Matrix * right, Matrix * result);
void matrix_sub_ (Matrix * left, const Matrix * right);
void matrix_subs (const Matrix * left, float right, Matrix * result);
void matrix_subs_ (Matrix * left, float right);
void matrix_tr (Matrix * input, const Matrix * transposed);
void matrix_zero_ (Matrix * matrix);

#endif
