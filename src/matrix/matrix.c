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
        rv += matrix->xs[i];
    }
    return rv;
}


void matrix_accdwn (const Matrix * matrix, Matrix * result) {
    assert(matrix->nc == result->nc && "expected number of columns in input to be equal to the number of columns in result");
    assert(result->nr == 1 && "expected number of rows in result to be equal to 1");
    for (size_t ic = 0; ic < matrix->nc; ic++) {
        result->xs[ic] = matrix->xs[ic];
    }
    for (size_t ir = 1; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            result->xs[ic] += matrix->xs[i];
        }
    }
}


void matrix_accrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nr && "expected number of rows in input to be equal to the number of rows in result");
    assert(result->nc == 1 && "expected number of columns in result to be equal to 1");
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        size_t i = ir * matrix->nc;
        result->xs[ir] = matrix->xs[i];
    }
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 1; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            result->xs[ir] += matrix->xs[i];
        }
    }
}


void matrix_addsca (const Matrix * left, const float right, Matrix * result) {
    for (size_t i = 0; i < left->n; i++) {
        result->xs[i] = left->xs[i] + right;
    }
}


float matrix_avgall (const Matrix * matrix) {
    float avg = 0.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        avg += matrix->xs[i] / matrix->n;
    }
    return avg;
}


void matrix_avgdwn (const Matrix * matrix, Matrix * result) {
    assert(result->nr == 1 && "expected number of rows in result to be 1");
    assert(matrix->nc == result->nc && "expected number of columns in matrix to be equal to number of columns in result");
    for (size_t ic = 0; ic < matrix->nc; ic++) {
        result->xs[ic] = matrix->xs[ic] / matrix->nr;
    }
    for (size_t ir = 1; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            result->xs[ic] += matrix->xs[i] / matrix->nr;
        }
    }
}


void matrix_avgrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nr && "expected number of rows in matrix to be equal to number of rows in result");
    assert(result->nc == 1 && "expected number of columns in result to be 1");
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        size_t i = ir * matrix->nc;
        result->xs[ir] = matrix->xs[i] / matrix->nc;
    }
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 1; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            result->xs[ir] += matrix->xs[i] / matrix->nc;
        }
    }

}


void matrix_bctdwn (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == 1 && "expected the number of rows in input to equal 1");
    assert(matrix->nc == result->nc && "expected the number of columns in the input to be equal to the number of columns in result");
    for (size_t ir = 0; ir < result->nr; ir++) {
        for (size_t ic = 0; ic < result->nc; ic++) {
            size_t i = ir * result->nc + ic;
            result->xs[i] = matrix->xs[ic];
        }
    }
}


void matrix_bctrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nc == 1 && "expected the number of columns in input to equal 1");
    assert(matrix->nr == result->nr && "expected the number of rows in the input to be equal to the number of rows in result");
    for (size_t ir = 0; ir < result->nr; ir++) {
        for (size_t ic = 0; ic < result->nc; ic++) {
            size_t i = ir * result->nc + ic;
            result->xs[i] = matrix->xs[ir];
        }
    }
}


Matrix * matrix_create (const size_t nr, const size_t nc) {
    errno = 0;
    MatrixResizable * m = calloc(1, sizeof(MatrixResizable));
    if (m == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for MatrixResizable instance, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }

    errno = 0;
    float * xs = calloc(nr * nc, sizeof(float));
    if (xs == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for values of MatrixResizable instance, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }

    m->nr = nr;
    m->nc = nc;
    m->n = nr * nc;
    m->xs = xs;
    return (Matrix *) m;
}


void matrix_divsca (const Matrix * matrix, const float factor, Matrix * result) {
    for (size_t i = 0; i < matrix->n; i++) {
        result->xs[i] = matrix->xs[i] / factor;
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
            result->xs[iresult] = 0.0f;
            for (size_t i = 0; i < n; i++) {
                size_t ileft = ir * left->nc + i;
                size_t iright = ic + i * right->nc;
                result->xs[iresult] += left->xs[ileft] * right->xs[iright];
            }
        }
    }
}


void matrix_destroy(Matrix ** matrix) {
    free((*matrix)->xs);
    (*matrix)->xs = nullptr;
    free(*matrix);
    *matrix = nullptr;
}


void matrix_ebeadd (const Matrix * left, const Matrix * right, Matrix * result) {
    assert(left->nr == right->nr && "Expected number of rows to be equal when adding 2 instances of Matrix");
    assert(left->nc == right->nc && "Expected number of columns to be equal when adding 2 instances of Matrix");
    for (size_t i = 0; i < left->n; i++) {
        result->xs[i] = left->xs[i] + right->xs[i];
    }
}


void matrix_ebediv (const Matrix * left, const Matrix * right, Matrix * result) {
    for (size_t i = 0; i < left->n; i++) {
        result->xs[i] = left->xs[i] / right->xs[i];
    }
}


void matrix_ebemap (const Matrix * matrix, float (*f) (float), Matrix * result) {
    assert(matrix->nr == result->nr && "Number of rows in input should match the number of rows in output");
    assert(matrix->nc == result->nc && "Number of columns in input should match the number of columns in output");
    for (size_t i = 0; i < matrix->n; i++) {
        result->xs[i] = f(matrix->xs[i]);
    }
}


void matrix_ebesub (const Matrix * left, const Matrix * right, Matrix * result) {
    assert(left->nr == right->nr && "Expected number of rows to be equal when subtracting 2 instances of Matrix");
    assert(left->nc == right->nc && "Expected number of columns to be equal when subtracting 2 instances of Matrix");
    for (size_t i = 0; i < left->n; i++) {
        result->xs[i] = left->xs[i] - right->xs[i];
    }
}


void matrix_hadpro (const Matrix * left, const Matrix * right, Matrix * result) {
    assert(left->nr == right->nr && "Number of rows in left operand should match the number of rows in right operand.");
    assert(left->nc == right->nc && "Number of columns in left operand should match the number of columns in right operand.");
    assert(left->nr == result->nr && "Number of rows in left operand should match the number of rows in the result.");
    assert(left->nc == result->nc && "Number of columns in left operand should match the number of columns in the result.");
    for (size_t i = 0; i < left->n; i++) {
        result->xs[i] = left->xs[i] * right->xs[i];
    }
}


void matrix_hstack (const Matrix * left, const Matrix * right, Matrix * result) {
    assert(left->nr == right->nr && "expected left and right number of rows to be equal");
    assert(left->nr == result->nr && "expected top number of rows to be equal to result's number of rows");
    size_t nr = left->nr;
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < left->nc; ic++) {
            size_t i = ir * result->nc + ic;
            size_t j = ir * left->nc + ic;
            result->xs[i] = left->xs[j];
        }
    }
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < right->nc; ic++) {
            size_t i = ir * result->nc + left->nc + ic;
            size_t j = ir * left->nc + ic;
            result->xs[i] = right->xs[j];
        }
    }
}


void matrix_ident (Matrix * matrix) {
    assert(matrix->nr == matrix->nc && "Expected a square matrix");
    size_t nr = matrix->nr;
    size_t nc = matrix->nc;
    for (size_t ir = 0; ir < nr; ir++) {
        for (size_t ic = 0; ic < nc; ic++) {
            size_t i = ir * nc + ic;
            if (ir == ic) {
                matrix->xs[i] = 1.0f;
            } else {
                matrix->xs[i] = 0.0f;
            }
        }
    }
}


float matrix_maxall (const Matrix * matrix) {
    float upper = matrix->xs[0];
    for (size_t i = 1; i < matrix->n; i++) {
        if (upper < matrix->xs[i]) {
            upper = matrix->xs[i];
        }
    }
    return upper;
}


void matrix_maxdwn (const Matrix * matrix, Matrix * result) {
    assert(matrix->nc == result->nc && "expected the number of columns in input to be equal to the number of columns in the result");
    assert(result->nr == 1 && "expected the number of rows in the result to be equal to 1");
    for (size_t ic = 0; ic < matrix->nc; ic++) {
        result->xs[ic] = matrix->xs[ic];
    }
    for (size_t ir = 1; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            if (matrix->xs[i] > result->xs[ic]) {
                result->xs[ic] = matrix->xs[i];
            }
        }
    }
}


void matrix_maxrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nr && "expected the number of rows in input to be equal to the number of rows in the result");
    assert(result->nc == 1 && "expected the number of columns in the result to be equal to 1");
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        size_t i = ir * matrix->nc;
        result->xs[ir] = matrix->xs[i];
    }
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 1; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            if (matrix->xs[i] > result->xs[ir]) {
                result->xs[ir] = matrix->xs[i];
            }
        }
    }
}


float matrix_minall (const Matrix * matrix) {
    float lower = matrix->xs[0];
    for (size_t i = 1; i < matrix->n; i++) {
        if (matrix->xs[i] < lower) {
            lower = matrix->xs[i];
        }
    }
    return lower;
}


void matrix_mindwn (const Matrix * matrix, Matrix * result) {
    assert(matrix->nc == result->nc && "expected the number of columns in input to be equal to the number of columns in the result");
    assert(result->nr == 1 && "expected the number of rows in the result to be equal to 1");
    for (size_t ic = 0; ic < matrix->nc; ic++) {
        result->xs[ic] = matrix->xs[ic];
    }
    for (size_t ir = 1; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            if (matrix->xs[i] < result->xs[ic]) {
                result->xs[ic] = matrix->xs[i];
            }
        }
    }
}


void matrix_minrgt (const Matrix * matrix, Matrix * result) {
    assert(matrix->nr == result->nr && "expected the number of rows in input to be equal to the number of rows in the result");
    assert(result->nc == 1 && "expected the number of columns in the result to be equal to 1");
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        size_t i = ir * matrix->nc;
        result->xs[ir] = matrix->xs[i];
    }
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 1; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            if (matrix->xs[i] < result->xs[ir]) {
                result->xs[ir] = matrix->xs[i];
            }
        }
    }
}


void matrix_ones (Matrix * matrix) {
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = 1.0f;
    }
}


void matrix_print (FILE * stream, const char * varname, const Matrix * matrix) {
    if (varname != nullptr && *varname != '\0') {
        fprintf(stream, "%s ", varname);
    }
    fprintf(stream, "(%zux%zu):\n", matrix->nr, matrix->nc);
    for (size_t ir = 0; ir < matrix->nr; ir++) {
        for (size_t ic = 0; ic < matrix->nc; ic++) {
            size_t i = ir * matrix->nc + ic;
            fprintf(stream, "%10g%s", matrix->xs[i], ic == matrix->nc - 1 ? "\n" : ", ");
        }
    }
}


size_t matrix_readnc (const char * filepath) {
    errno = 0;
    FILE * fp = fopen(filepath, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError opening file '%s', aborting.\n", strerror(errno), filepath);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    constexpr size_t cap = 4096;
    char buffer[cap] = {};
    char * status = fgets(buffer, cap, fp);
    if (status == nullptr) {
        fprintf(stderr, "ERROR reading number of columns from file \"%s\", aborting.\n", filepath);
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    size_t nr = 0;
    size_t nc = 0;
    sscanf(buffer, "%*s (%zux%zu):\n", &nr, &nc);
    return nc;
}


size_t matrix_readnr (const char * filepath) {
    errno = 0;
    FILE * fp = fopen(filepath, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError opening file '%s', aborting.\n", strerror(errno), filepath);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    constexpr size_t cap = 4096;
    char buffer[cap] = {};
    char * status = fgets(buffer, cap, fp);
    if (status == nullptr) {
        fprintf(stderr, "ERROR reading number of rows from file \"%s\", aborting.\n", filepath);
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    size_t nr = 0;
    size_t nc = 0;
    sscanf(buffer, "%*s (%zux%zu):\n", &nr, &nc);
    return nr;
}


void matrix_readxs (const char * filepath, Matrix * results) {
    errno = 0;
    FILE * fp = fopen(filepath, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError opening file '%s', aborting.\n", strerror(errno), filepath);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    constexpr size_t cap = 4096;
    char buffer[cap] = {};
    char * status = fgets(buffer, cap, fp);
    if (status == nullptr) {
        fprintf(stderr, "ERROR reading number of first line from file \"%s\", aborting.\n", filepath);
        exit(EXIT_FAILURE);
    }
    for (size_t ir = 0; ir < results->nr; ir++) {
        char * status = fgets(buffer, cap, fp);
        if (status == nullptr) {
            fprintf(stderr, "ERROR reading line %zu from file \"%s\", aborting.\n", ir, filepath);
            exit(EXIT_FAILURE);
        }
        char * buffer_p = &buffer[0];
        char * next = nullptr;
        char * token = nullptr;
        for (size_t ic = 0; ic < results->nc; ic++) {
            token = strtok_r(buffer_p, ", \n", &next);
            if (token == nullptr) {
                fprintf(stderr, "ERROR reading token %zu on line %zu from file \"%s\", aborting.\n", ic, ir, filepath);
                exit(EXIT_FAILURE);
            }
            size_t i = ir * results->nc + ic;
            int nscanned = sscanf(token, " %f", &results->xs[i]);
            if (nscanned != 1) {
                fprintf(stderr, "ERROR scanning token %zu on line %zu from file \"%s\", aborting.\n", ic, ir, filepath);
                exit(EXIT_FAILURE);
            }
            buffer_p = nullptr;  // (strtok_r magic)
        }
    }
    fclose(fp);
}


void matrix_scapro (const Matrix * left, const float right, Matrix * result) {
    for (size_t i = 0; i < left->n; i++) {
        result->xs[i] = left->xs[i] * right;
    }
}


void matrix_stzall (const Matrix * matrix, float * avg, float * stddev, float * result) {
    assert(matrix != nullptr && "dummy assert");
    assert(avg != nullptr && "dummy assert");
    assert(stddev != nullptr && "dummy assert");
    assert(result != nullptr && "dummy assert");
    assert(false && "Not implemented");
}


void matrix_stzdwn (const Matrix * matrix, Matrix * avgs, Matrix * stddevs, Matrix * result) {
    assert(matrix->nr == result->nr && "number of rows in input should be same as number of rows in output");
    assert(matrix->nc == result->nc && "number of columns in input should be same as number of columns in output");
    size_t nr = matrix->nr;
    size_t nc = matrix->nc;
    Matrix * avgs_bctdwn = matrix_create(nr, nc);
    Matrix * stddevs_bctdwn = matrix_create(nr, nc);
    {
        Matrix * deviations = matrix_create(nr, nc);
        Matrix * deviations_sq = matrix_create(nr, nc);
        Matrix * tmp = matrix_create(1, nc);

        matrix_avgdwn(matrix, avgs);
        matrix_bctdwn(avgs, avgs_bctdwn);
        matrix_ebesub(avgs_bctdwn, matrix, deviations);
        matrix_hadpro(deviations, deviations, deviations_sq);
        matrix_avgdwn(deviations_sq, tmp);
        matrix_ebemap(tmp, sqrtf, stddevs);

        matrix_destroy(&tmp);
        matrix_destroy(&deviations_sq);
        matrix_destroy(&deviations);
    }
    {
        matrix_bctdwn(stddevs, stddevs_bctdwn);
        matrix_ebesub(matrix, avgs_bctdwn, result);
        matrix_ebediv(result, stddevs_bctdwn, result);
    }
    matrix_destroy(&stddevs_bctdwn);
    matrix_destroy(&avgs_bctdwn);
}


void matrix_stzrgt (const Matrix * matrix, Matrix * avgs, Matrix * stddevs, Matrix * result) {
    assert(matrix->nr == result->nr && "number of rows in input should be same as number of rows in output");
    assert(matrix->nc == result->nc && "number of columns in input should be same as number of columns in output");
    size_t nr = matrix->nr;
    size_t nc = matrix->nc;
    Matrix * avgs_bctrgt = matrix_create(nr, nc);
    Matrix * stddevs_bctrgt = matrix_create(nr, nc);
    {
        Matrix * deviations = matrix_create(nr, nc);
        Matrix * deviations_sq = matrix_create(nr, nc);
        Matrix * tmp = matrix_create(nr, 1);

        matrix_avgrgt(matrix, avgs);
        matrix_bctrgt(avgs, avgs_bctrgt);
        matrix_ebesub(avgs_bctrgt, matrix, deviations);
        matrix_hadpro(deviations, deviations, deviations_sq);
        matrix_avgrgt(deviations_sq, tmp);
        matrix_ebemap(tmp, sqrtf, stddevs);

        matrix_destroy(&tmp);
        matrix_destroy(&deviations_sq);
        matrix_destroy(&deviations);
    }
    {
        matrix_bctrgt(stddevs, stddevs_bctrgt);
        matrix_ebesub(matrix, avgs_bctrgt, result);
        matrix_ebediv(result, stddevs_bctrgt, result);
    }
    matrix_destroy(&stddevs_bctrgt);
    matrix_destroy(&avgs_bctrgt);
}


void matrix_subsca (const Matrix * left, const float right, Matrix * result) {
    for (size_t i = 0; i < left->n; i++) {
        result->xs[i] = left->xs[i] - right;
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


bool matrix_testeq (const Matrix * a, const Matrix * b, const float eps) {
    assert(a->nr == b->nr && "Number of rows in 'a' should match number of rows in 'b'");
    assert(a->nc == b->nc && "Number of columns in 'a' should match number of columns in 'b'");
    for (size_t i = 0; i < a->n; i++) {
        float ai = a->xs[i];
        float bi = b->xs[i];
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
            result->xs[j] = matrix->xs[i];
        }
    }
}


void matrix_ustzall (const Matrix * matrix, float * avg, float * stddev, float * result) {
    assert(matrix != nullptr && "dummy assert");
    assert(avg != nullptr && "dummy assert");
    assert(stddev != nullptr && "dummy assert");
    assert(result != nullptr && "dummy assert");
    assert(false && "Not implemented");
}


void matrix_ustzdwn (const Matrix * matrix, const Matrix * avgs, const Matrix * stddevs, Matrix * result) {
    assert(matrix != nullptr && "dummy assert");
    assert(avgs != nullptr && "dummy assert");
    assert(stddevs != nullptr && "dummy assert");
    assert(result != nullptr && "dummy assert");
    assert(false && "Not implemented");
}


void matrix_ustzrgt (const Matrix * matrix, const Matrix * avgs, const Matrix * stddevs, Matrix * result) {
    assert(matrix != nullptr && "dummy assert");
    assert(avgs != nullptr && "dummy assert");
    assert(stddevs != nullptr && "dummy assert");
    assert(result != nullptr && "dummy assert");
    assert(false && "Not implemented");
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
            tmp->xs[ir] = matrix->xs[i];
        }
        result->xs[ic] = matrix_varall(tmp);
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
            tmp->xs[ic] = matrix->xs[i];
        }
        result->xs[ir] = matrix_varall(tmp);
        matrix_destroy(&tmp);
    }
}


void matrix_vstack (const Matrix * top, const Matrix * bottom, Matrix * result) {
    assert(top->nc == bottom->nc && "expected top and bottom number of columns to be equal");
    assert(top->nc == result->nc && "expected top number of columns to be equal to result's number of columns");
    for (size_t i = 0; i < top->n; i++) {
        result->xs[i] = top->xs[i];
    }
    for (size_t i = 0; i < bottom->n; i++) {
        result->xs[top->n + i] = bottom->xs[i];
    }
}


void matrix_write (const char * basename, const char * varname, const Matrix * matrix) {
    constexpr size_t ell = 128 + 128 + 4 + 1;
    char filename[ell] = { '\0' };
    const char extension[5] = ".txt";
    char * p = &filename[0];
    size_t n0 = strlen(basename) < 128 ? strlen(basename) : 128;
    size_t n1 = strlen(varname) < 128 ? strlen(varname) : 128;
    size_t n2 = strlen(extension);
    for (size_t i = 0; i < n0; i++, p++) {
        *p = basename[i];
    }
    for (size_t i = 0; i < n1; i++, p++) {
        *p = varname[i];
    }
    for (size_t i = 0; i < n2; i++, p++) {
        *p = extension[i];
    }
    *(++p) = '\0';
    errno = 0;
    FILE * fp = fopen(filename, "w+");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError opening file '%s', aborting.\n", strerror(errno), filename);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    matrix_print(fp, varname, matrix);
    fclose(fp);
}


void matrix_zeros (Matrix * matrix) {
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = 0.0f;
    }
}
