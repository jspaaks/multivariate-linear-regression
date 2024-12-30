#include "matrix/matrix.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


float matrix_accall (const Matrix * matrix) {
    float rv = 0.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        rv += matrix->vals[i];
    }
    return rv;
}


void matrix_accdwn (const Matrix * matrix, Matrix * result) {
    assert(matrix->nc == result->nc && "expected number of columns in input to be equal to the number of columns in result");
    assert(result->nr == 1 && "expected number of rows in result to be equal to 1");
    for (size_t ic = 0; ic < matrix->nc; ic++) {
        result->vals[ic] = matrix->vals[ic];
    }
    for (size_t ir = 1; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            result->vals[ic] += matrix->vals[i];
        }
    }
}


void matrix_accrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nr && "expected number of rows in input to be equal to the number of rows in result");
    assert(result->nc == 1 && "expected number of columns in result to be equal to 1");
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        size_t i = ir * matrix->nc;
        result->vals[ir] = matrix->vals[i];
    }
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 1; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            result->vals[ir] += matrix->vals[i];
        }
    }
}


void matrix_addsca (const Matrix * left, float right, Matrix * result) {
    for (size_t i = 0; i < left->n; i++) {
        result->vals[i] = left->vals[i] + right;
    }
}


float matrix_avgall (const Matrix * matrix) {
    float avg = 0.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        avg += matrix->vals[i] / matrix->n;
    }
    return avg;
}


void matrix_avgdwn (const Matrix * matrix, Matrix * result) {
    assert(result->nr == 1 && "expected number of rows in result to be 1");
    assert(matrix->nc == result->nc && "expected number of columns in matrix to be equal to number of columns in result");
    for (size_t ic = 0; ic < matrix->nc; ic++) {
        result->vals[ic] = matrix->vals[ic] / matrix->nr;
    }
    for (size_t ir = 1; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            result->vals[ic] += matrix->vals[i] / matrix->nr;
        }
    }
}


void matrix_avgrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nr && "expected number of rows in matrix to be equal to number of rows in result");
    assert(result->nc == 1 && "expected number of columns in result to be 1");
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        size_t i = ir * matrix->nc;
        result->vals[ir] = matrix->vals[i] / matrix->nc;
    }
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 1; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            result->vals[ir] += matrix->vals[i] / matrix->nc;
        }
    }

}


void matrix_bctdwn (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == 1 && "expected the number of rows in input to equal 1");
    assert(matrix->nc == result->nc && "expected the number of columns in the input to be equal to the number of columns in result");
    for (size_t ir = 0; ir < result->nr; ir++) {
        for (size_t ic = 0; ic < result->nc; ic++) {
            size_t i = ir * result->nc + ic;
            result->vals[i] = matrix->vals[ic];
        }
    }
}


void matrix_bctrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nc == 1 && "expected the number of columns in input to equal 1");
    assert(matrix->nr == result->nr && "expected the number of rows in the input to be equal to the number of rows in result");
    for (size_t ir = 0; ir < result->nr; ir++) {
        for (size_t ic = 0; ic < result->nc; ic++) {
            size_t i = ir * result->nc + ic;
            result->vals[i] = matrix->vals[ir];
        }
    }
}


Matrix * matrix_create (size_t nr, size_t nc) {
    errno = 0;
    MatrixResizable * m = calloc(1, sizeof(MatrixResizable));
    if (m == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for MatrixResizable instance, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }

    errno = 0;
    float * vals = calloc(nr * nc, sizeof(float));
    if (vals == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for values of MatrixResizable instance, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }

    m->nr = nr;
    m->nc = nc;
    m->n = nr * nc;
    m->vals = vals;
    return (Matrix *) m;
}


void matrix_divsca (const Matrix * matrix, float factor, Matrix * result) {
    for (size_t i = 0; i < matrix->n; i++) {
        result->vals[i] = matrix->vals[i] / factor;
    }
}


void matrix_dotpro (const Matrix * left, const Matrix * right, Matrix * result) {
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


void matrix_destroy(Matrix ** matrix) {
    free((*matrix)->vals);
    (*matrix)->vals = nullptr;
    free(*matrix);
    *matrix = nullptr;
}


void matrix_ebeadd (const Matrix * left, const Matrix * right, Matrix * result) {
    assert(left->nr == right->nr && "Expected number of rows to be equal when adding 2 instances of Matrix");
    assert(left->nc == right->nc && "Expected number of columns to be equal when adding 2 instances of Matrix");
    for (size_t i = 0; i < left->n; i++) {
        result->vals[i] = left->vals[i] + right->vals[i];
    }
}


void matrix_ebediv (const Matrix * left, const Matrix * right, Matrix * result) {
    for (size_t i = 0; i < left->n; i++) {
        result->vals[i] = left->vals[i] / right->vals[i];
    }
}


void matrix_ebemap (const Matrix * matrix, float (*f) (float), Matrix * result) {
    assert(matrix->nr == result->nr && "Number of rows in input should match the number of rows in output");
    assert(matrix->nc == result->nc && "Number of columns in input should match the number of columns in output");
    for (size_t i = 0; i < matrix->n; i++) {
        result->vals[i] = f(matrix->vals[i]);
    }
}


void matrix_ebesub (const Matrix * left, const Matrix * right, Matrix * result) {
    assert(left->nr == right->nr && "Expected number of rows to be equal when subtracting 2 instances of Matrix");
    assert(left->nc == right->nc && "Expected number of columns to be equal when subtracting 2 instances of Matrix");
    for (size_t i = 0; i < left->n; i++) {
        result->vals[i] = left->vals[i] - right->vals[i];
    }
}


void matrix_hadpro (const Matrix * left, const Matrix * right, Matrix * result) {
    assert(left->nr == right->nr && "Number of rows in left operand should match the number of rows in right operand.");
    assert(left->nc == right->nc && "Number of columns in left operand should match the number of columns in right operand.");
    assert(left->nr == result->nr && "Number of rows in left operand should match the number of rows in the result.");
    assert(left->nc == result->nc && "Number of columns in left operand should match the number of columns in the result.");
    for (size_t i = 0; i < left->n; i++) {
        result->vals[i] = left->vals[i] * right->vals[i];
    }
}


float matrix_maxall (const Matrix * matrix) {
    float upper = matrix->vals[0];
    for (size_t i = 1; i < matrix->n; i++) {
        if (upper < matrix->vals[i]) {
            upper = matrix->vals[i];
        }
    }
    return upper;
}


void matrix_maxdwn (const Matrix * matrix, Matrix * result) {
    assert(matrix->nc == result->nc && "expected the number of columns in input to be equal to the number of columns in the result");
    assert(result->nr == 1 && "expected the number of rows in the result to be equal to 1");
    for (size_t ic = 0; ic < matrix->nc; ic++) {
        result->vals[ic] = matrix->vals[ic];
    }
    for (size_t ir = 1; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            if (matrix->vals[i] > result->vals[ic]) {
                result->vals[ic] = matrix->vals[i];
            }
        }
    }
}


void matrix_maxrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nr && "expected the number of rows in input to be equal to the number of rows in the result");
    assert(result->nc == 1 && "expected the number of columns in the result to be equal to 1");
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        size_t i = ir * matrix->nc;
        result->vals[ir] = matrix->vals[i];
    }
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 1; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            if (matrix->vals[i] > result->vals[ir]) {
                result->vals[ir] = matrix->vals[i];
            }
        }
    }
}


float matrix_minall (const Matrix * matrix) {
    float lower = matrix->vals[0];
    for (size_t i = 1; i < matrix->n; i++) {
        if (matrix->vals[i] < lower) {
            lower = matrix->vals[i];
        }
    }
    return lower;
}


void matrix_mindwn (const Matrix * matrix, Matrix * result) {
    assert(matrix->nc == result->nc && "expected the number of columns in input to be equal to the number of columns in the result");
    assert(result->nr == 1 && "expected the number of rows in the result to be equal to 1");
    for (size_t ic = 0; ic < matrix->nc; ic++) {
        result->vals[ic] = matrix->vals[ic];
    }
    for (size_t ir = 1; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            if (matrix->vals[i] < result->vals[ic]) {
                result->vals[ic] = matrix->vals[i];
            }
        }
    }
}


void matrix_minrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nr && "expected the number of rows in input to be equal to the number of rows in the result");
    assert(result->nc == 1 && "expected the number of columns in the result to be equal to 1");
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        size_t i = ir * matrix->nc;
        result->vals[ir] = matrix->vals[i];
    }
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 1; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            if (matrix->vals[i] < result->vals[ir]) {
                result->vals[ir] = matrix->vals[i];
            }
        }
    }
}


void matrix_print (FILE * stream, Matrix * matrix, char * description) {
    if (description != nullptr && *description != '\0') {
        fprintf(stream, "%s ", description);
    }
    fprintf(stream, "(%zux%zu):\n", matrix->nr, matrix->nc);
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            fprintf(stream, "%10g%s", matrix->vals[i], ic == matrix->nc - 1 ? "\n" : ", ");
        }
    }
}


void matrix_scapro (const Matrix * left, float right, Matrix * result) {
    for (size_t i = 0; i < left->n; i++) {
        result->vals[i] = left->vals[i] * right;
    }
}


void matrix_stzdwn (const Matrix * matrix, Matrix * avgs, Matrix * stddevs, Matrix * result) {
    size_t nr = matrix->nr;
    size_t nc = matrix->nc;
    Matrix * avgs_bcastd = matrix_create(nr, nc);
    Matrix * stddevs_bcastd = matrix_create(nr, nc);
    {
        Matrix * deviations = matrix_create(nr, nc);
        Matrix * deviations_sq = matrix_create(nr, nc);
        Matrix * tmp = matrix_create(1, nc);

        matrix_avgdwn(matrix, avgs);
        matrix_bctdwn(avgs, avgs_bcastd);
        matrix_ebesub(matrix, avgs_bcastd, deviations);
        matrix_hadpro(deviations, deviations, deviations_sq);
        matrix_accdwn(deviations_sq, tmp);
        matrix_ebemap(tmp, sqrtf, stddevs);

        matrix_destroy(&tmp);
        matrix_destroy(&deviations_sq);
        matrix_destroy(&deviations);
    }
    {
        matrix_bctdwn(stddevs, stddevs_bcastd);
        matrix_ebesub(matrix, avgs_bcastd, result);
        matrix_ebediv(result, stddevs_bcastd, result);
    }
    matrix_destroy(&stddevs_bcastd);
    matrix_destroy(&avgs_bcastd);
}


void matrix_subsca (const Matrix * left, float right, Matrix * result) {
    for (size_t i = 0; i < left->n; i++) {
        result->vals[i] = left->vals[i] - right;
    }
}


float matrix_sdvall (const Matrix * matrix) {
    return sqrt(matrix_varall(matrix));
}


void matrix_sdvdwn (const Matrix * matrix, Matrix * result) {
    matrix_vardwn(matrix, result);
    matrix_ebemap(result, sqrtf, result);
}


void matrix_sdvrgt (const Matrix * matrix, Matrix * result) {
    matrix_varrgt(matrix, result);
    matrix_ebemap(result, sqrtf, result);
}


bool matrix_testeq (const Matrix * a, const Matrix * b, float eps) {
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


void matrix_transp (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nc && "Number of rows in input should be equal to the number of columns in the output.");
    assert(matrix->nc == result->nr && "Number of columns in input should be equal to the number of rows in the output.");
    size_t nr = matrix->nr;
    size_t nc = matrix->nc;
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < nc; ic++) {
            size_t i = ir * nc + ic;
            size_t j = ic * nr + ir;
            result->vals[j] = matrix->vals[i];
        }
    }
}


float matrix_varall (const Matrix * matrix) {
    size_t nr = matrix->nr;
    size_t nc = matrix->nc;
    Matrix * residuals = matrix_create(nr, nc);
    Matrix * residuals2 = matrix_create(nr, nc);
    float avg = matrix_avgall(matrix);
    matrix_subsca(matrix, avg, residuals);
    matrix_hadpro(residuals, residuals, residuals2);
    float rv = matrix_avgall(residuals2);
    matrix_destroy(&residuals);
    matrix_destroy(&residuals2);
    return rv;
}


void matrix_vardwn (const Matrix * matrix, Matrix * result) {
    assert(matrix->nc == result->nc && "Number of columns in input should be equal to the number of columns in the output.");
    assert(result->nr == 1 && "Number of rows in the output should be 1.");
    size_t nr = matrix->nr;
    size_t nc = matrix->nc;
    for (size_t ic = 0; ic < nc; ic++) {
        Matrix * tmp = matrix_create(nr, 1);
        for (size_t ir = 0; ir < nr; ir++) {
            size_t i = ir * nc + ic;
            tmp->vals[ir] = matrix->vals[i];
        }
        result->vals[ic] = matrix_varall(tmp);
        matrix_destroy(&tmp);
    }
}


void matrix_varrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nr && "Number of rows in input should be equal to the number of rows in the output.");
    assert(result->nc == 1 && "Number of columns in the output should be 1.");
    size_t nr = matrix->nr;
    size_t nc = matrix->nc;
    for (size_t ir = 0; ir < nr; ir++) {
        Matrix * tmp = matrix_create(1, nr);
        for (size_t ic = 0; ic < nc; ic++) {
            size_t i = ir * nc + ic;
            tmp->vals[ic] = matrix->vals[i];
        }
        result->vals[ir] = matrix_varall(tmp);
        matrix_destroy(&tmp);
    }
}


void matrix_zero (Matrix * matrix) {
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = 0.0f;
    }
}
