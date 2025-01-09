#include "matrix/matrix.h"
#include <criterion/criterion.h>

void derive_limits_of_acceptability(float expected, float tolerance, float * lower, float * upper);

void derive_limits_of_acceptability(float expected, float tolerance, float * lower, float * upper) {
    *lower = expected - tolerance;
    *upper = expected + tolerance;
}


Test(matrix, accall) {
    Matrix * matrix = matrix_create(3, 2);
    matrix->xs[0] =   2.0f;
    matrix->xs[1] =   4.0f;
    matrix->xs[2] =  12.0f;
    matrix->xs[3] =  22.0f;
    matrix->xs[4] = 452.0f;
    matrix->xs[5] =  -4.0f;
    float expected  = 488.0f;
    float tolerance =   0.1f;
    float lower;
    float upper;
    derive_limits_of_acceptability(expected, tolerance, &lower, &upper);
    float actual = matrix_accall(matrix);
    cr_assert(lower < actual && actual < upper, "expected actual to be equal to %.1f within the given tolerance (was %.1f)", expected, actual);
    matrix_destroy(&matrix);
}


Test(matrix, accdwn) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * actual = matrix_create(1, 2);
    Matrix * expected = matrix_create(1, 2);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }

    expected->xs[0] = 2.0f;
    expected->xs[1] = 4.0f;

    matrix_accdwn(matrix, actual);

    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, accrgt) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * actual = matrix_create(2, 1);
    Matrix * expected = matrix_create(2, 1);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }

    expected->xs[0] = 1.0f;
    expected->xs[1] = 5.0f;

    matrix_accrgt(matrix, actual);

    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, addsca) {
    Matrix * left = matrix_create(2, 2);
    float right = 10.0f;
    Matrix * actual = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);

    for (size_t i = 0; i < left->n; i++) {
        left->xs[i] = i;
        expected->xs[i] = 10 + i;
    }
    matrix_addsca(left, right, actual);
    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&left);
}


Test(matrix, avgall) {
    Matrix * matrix = matrix_create(2, 3);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }
    float expected = 2.5f;
    float tolerance = 0.0001f;
    float lower;
    float upper;
    derive_limits_of_acceptability(expected, tolerance, &lower, &upper);
    float actual = matrix_avgall(matrix);
    cr_assert(lower < actual && actual < upper, "expected actual to be equal to %.1f within the given tolerance (was %.1f)", expected, actual);
    matrix_destroy(&matrix);
}


Test(matrix, avgdwn) {
    Matrix * matrix = matrix_create(2, 3);
    Matrix * actual = matrix_create(1, 3);
    Matrix * expected = matrix_create(1, 3);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }

    for (size_t i = 0; i < expected->n; i++) {
        expected->xs[i] = i + 1.5f;
    }

    matrix_avgdwn(matrix, actual);

    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, avgrgt) {
    Matrix * matrix = matrix_create(3, 2);
    Matrix * actual = matrix_create(3, 1);
    Matrix * expected = matrix_create(3, 1);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }

    for (size_t i = 0; i < expected->n; i++) {
        expected->xs[i] = 2 * i + 0.5f;
    }

    matrix_avgrgt(matrix, actual);

    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, bctdwn) {
    Matrix * matrix = matrix_create(1, 3);
    Matrix * actual = matrix_create(4, 3);
    Matrix * expected = matrix_create(4, 3);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }

    for (size_t ir = 0; ir < expected->nr; ir++) {
        for (size_t ic = 0; ic < expected->nc; ic++) {
            size_t i = ir * expected->nc + ic;
            expected->xs[i] = ic;
        }
    }

    matrix_bctdwn(matrix, actual);

    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, bctrgt) {
    Matrix * matrix = matrix_create(3, 1);
    Matrix * actual = matrix_create(3, 4);
    Matrix * expected = matrix_create(3, 4);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }

    for (size_t ir = 0; ir < expected->nr; ir++) {
        for (size_t ic = 0; ic < expected->nc; ic++) {
            size_t i = ir * expected->nc + ic;
            expected->xs[i] = ir;
        }
    }

    matrix_bctrgt(matrix, actual);

    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, divsca) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * actual = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);
    float factor = 10.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
        expected->xs[i] = i / factor;
    }
    matrix_divsca(matrix, factor, actual);
    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, dotpro) {
    Matrix * a = matrix_create(2, 3);
    Matrix * b = matrix_create(3, 4);
    Matrix * c = matrix_create(2, 4);
    for (size_t i = 0; i < a->n; i++) {
        a->xs[i] = i;
    }
    for (size_t i = 0; i < b->n; i++) {
        b->xs[i] = i + 10;
    }
    matrix_dotpro(a, b, c);
    const float expected[8] = {  50.0f,  53.0f,  56.0f,  59.0f, 176.0f, 188.0f, 200.0f, 212.0f};
    const size_t n = c->nr * c->nc;
    for (size_t i = 0; i < n; i++) {
        const float lower = expected[i] - 0.01f;
        const float upper = expected[i] + 0.01f;
        const float actual = c->xs[i];
        cr_assert(lower < actual && actual < upper, "actual was %.6f", actual);
    }
    matrix_destroy(&c);
    matrix_destroy(&b);
    matrix_destroy(&a);
}


Test(matrix, ebeadd) {
    Matrix * left = matrix_create(2, 2);
    Matrix * right = matrix_create(2, 2);
    Matrix * actual = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);

    for (size_t i = 0; i < left->n; i++) {
        left->xs[i] = i;
        right->xs[i] = 10;
        expected->xs[i] = 10 + i;
    }
    matrix_ebeadd(left, right, actual);
    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, ebediv) {
}


Test(matrix, ebemap) {
}


Test(matrix, ebesub) {
    Matrix * left = matrix_create(2, 3);
    Matrix * right = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);
    for (size_t i = 0; i < left->n; i++) {
        left->xs[i] = 10 + i;
        right->xs[i] = i;
        expected->xs[i] = 10;
    }
    matrix_ebesub(left, right, actual);
    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, hadpro) {
    Matrix * left = matrix_create(2, 3);
    Matrix * right = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);

    for (size_t ir = 0; ir < left->nr; ir++) {
        for (size_t ic = 0; ic < left->nc; ic++) {
            size_t i = ir * left->nc + ic;
            left->xs[i] = ic + 1;
            right->xs[i] = ir + 1;
            expected->xs[i] = (ic + 1) * (ir + 1);
        }
    }
    matrix_hadpro(left, right, actual);
    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, hstack) {
    Matrix * left = matrix_create(2, 3);
    Matrix * right = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 6);
    Matrix * expected = matrix_create(2, 6);

    left->xs[0] = 1.0f;
    left->xs[1] = 2.0f;
    left->xs[2] = 3.0f;
    left->xs[3] = 7.0f;
    left->xs[4] = 8.0f;
    left->xs[5] = 9.0f;

    right->xs[0] =  4.0f;
    right->xs[1] =  5.0f;
    right->xs[2] =  6.0f;
    right->xs[3] = 10.0f;
    right->xs[4] = 11.0f;
    right->xs[5] = 12.0f;

    matrix_hstack(left, right, actual);

    expected->xs[0] =   1.0f;
    expected->xs[1] =   2.0f;
    expected->xs[2] =   3.0f;
    expected->xs[3] =   4.0f;
    expected->xs[4] =   5.0f;
    expected->xs[5] =   6.0f;
    expected->xs[6] =   7.0f;
    expected->xs[7] =   8.0f;
    expected->xs[8] =   9.0f;
    expected->xs[9] =  10.0f;
    expected->xs[10] = 11.0f;
    expected->xs[11] = 12.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "vstack failed");

    matrix_destroy(&left);
    matrix_destroy(&right);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, ident) {
    Matrix * expected = matrix_create(3, 3);
    Matrix * ident = matrix_create(3, 3);

    matrix_ident(ident);

    expected->xs[0] = 1.0f;
    expected->xs[1] = 0.0f;
    expected->xs[2] = 0.0f;
    expected->xs[3] = 0.0f;
    expected->xs[4] = 1.0f;
    expected->xs[5] = 0.0f;
    expected->xs[6] = 0.0f;
    expected->xs[7] = 0.0f;
    expected->xs[8] = 1.0f;

    bool cond = matrix_testeq(ident, expected, 0.01f);
    cr_assert(cond, "matrix_ident failed");

    matrix_destroy(&expected);
    matrix_destroy(&ident);
}


Test(matrix, maxall) {
    Matrix * matrix = matrix_create(3, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }
    float actual = matrix_maxall(matrix);
    float expected = 5.0f;
    float tolerance = 0.001f;
    float lower;
    float upper;
    derive_limits_of_acceptability(expected, tolerance, &lower, &upper);
    cr_assert(lower < actual && actual < upper, "expected actual to be equal to %.1f within the given tolerance (was %.1f)", expected, actual);
    matrix_destroy(&matrix);
}


Test(matrix, maxdwn) {
    Matrix * matrix = matrix_create(3, 2);
    Matrix * actual = matrix_create(1, 2);
    Matrix * expected = matrix_create(1, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }
    expected->xs[0] = 4.0f;
    expected->xs[1] = 5.0f;
    matrix_maxdwn(matrix, actual);
    bool cond = matrix_testeq(actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, maxrgt) {
    Matrix * matrix = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 1);
    Matrix * expected = matrix_create(2, 1);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }
    expected->xs[0] = 2.0f;
    expected->xs[1] = 5.0f;
    matrix_maxrgt(matrix, actual);
    bool cond = matrix_testeq(actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, minall) {
    Matrix * matrix = matrix_create(3, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }
    float actual = matrix_minall(matrix);
    float expected = 0.0f;
    float tolerance = 0.001f;
    float lower;
    float upper;
    derive_limits_of_acceptability(expected, tolerance, &lower, &upper);
    cr_assert(lower < actual && actual < upper, "expected actual to be equal to %.1f within the given tolerance (was %.1f)", expected, actual);
    matrix_destroy(&matrix);
}


Test(matrix, mindwn) {
    Matrix * matrix = matrix_create(3, 2);
    Matrix * actual = matrix_create(1, 2);
    Matrix * expected = matrix_create(1, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }
    expected->xs[0] = 0.0f;
    expected->xs[1] = 1.0f;
    matrix_mindwn(matrix, actual);
    bool cond = matrix_testeq(actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, minrgt) {
    Matrix * matrix = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 1);
    Matrix * expected = matrix_create(2, 1);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
    }
    expected->xs[0] = 0.0f;
    expected->xs[1] = 3.0f;
    matrix_minrgt(matrix, actual);
    bool cond = matrix_testeq(actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}



Test(matrix, ones) {
    Matrix * actual = matrix_create(2, 3);
    actual->xs[0] = 1.0f;
    actual->xs[1] = 2.0f;
    actual->xs[2] = 3.0f;
    actual->xs[3] = 4.0f;
    actual->xs[4] = 5.0f;
    actual->xs[5] = 6.0f;

    matrix_ones(actual);

    Matrix * expected = matrix_create(2, 3);
    expected->xs[0] = 1.0f;
    expected->xs[1] = 1.0f;
    expected->xs[2] = 1.0f;
    expected->xs[3] = 1.0f;
    expected->xs[4] = 1.0f;
    expected->xs[5] = 1.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "ones failed");

    matrix_destroy(&expected);
    matrix_destroy(&actual);
}


Test(matrix, scapro) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * actual = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);
    float factor = 10.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->xs[i] = i;
        expected->xs[i] = i * factor;
    }
    matrix_scapro(matrix, factor, actual);
    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, sdvall) {
    Matrix * matrix = matrix_create(1, 8);
    matrix->xs[0] = 2.0f;
    matrix->xs[1] = 4.0f;
    matrix->xs[2] = 4.0f;
    matrix->xs[3] = 4.0f;
    matrix->xs[4] = 5.0f;
    matrix->xs[5] = 5.0f;
    matrix->xs[6] = 7.0f;
    matrix->xs[7] = 9.0f;
    float actual = matrix_sdvall(matrix);
    float expected = 2.0f;
    float tolerance = 0.001f;
    float lower;
    float upper;
    derive_limits_of_acceptability(expected, tolerance, &lower, &upper);
    cr_assert(lower < actual && actual < upper, "expected actual to be equal to %.1f within the given tolerance (was %.1f)", expected, actual);
    matrix_destroy(&matrix);
}


Test(matrix, sdvdwn) {
    Matrix * matrix = matrix_create(3, 3);
    Matrix * actual = matrix_create(1, 3);
    Matrix * expected = matrix_create(1, 3);

    matrix->xs[0] = 0.0f;
    matrix->xs[1] = 1.0f;
    matrix->xs[2] = 2.0f;
    matrix->xs[3] = 3.0f;
    matrix->xs[4] = 5.0f;
    matrix->xs[5] = 7.0f;
    matrix->xs[6] = 8.0f;
    matrix->xs[7] = 11.0f;
    matrix->xs[8] = 14.0f;

    expected->xs[0] = 3.300;
    expected->xs[1] = 4.110;
    expected->xs[2] = 4.922;

    matrix_sdvdwn(matrix, actual);

    bool cond = matrix_testeq(actual, expected, 0.01);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&matrix);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, sdvrgt) {
    Matrix * matrix = matrix_create(3, 3);
    Matrix * actual = matrix_create(3, 1);
    Matrix * expected = matrix_create(3, 1);

    matrix->xs[0] = 0.0f;
    matrix->xs[1] = 1.0f;
    matrix->xs[2] = 2.0f;
    matrix->xs[3] = 3.0f;
    matrix->xs[4] = 5.0f;
    matrix->xs[5] = 7.0f;
    matrix->xs[6] = 8.0f;
    matrix->xs[7] = 11.0f;
    matrix->xs[8] = 14.0f;

    expected->xs[0] = 0.816;
    expected->xs[1] = 1.633;
    expected->xs[2] = 2.449;

    matrix_sdvrgt(matrix, actual);

    bool cond = matrix_testeq(actual, expected, 0.01);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&matrix);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, stzdwn) {
    Matrix * matrix = matrix_create(3, 2);
    Matrix * actual = matrix_create(3, 2);
    Matrix * expected = matrix_create(3, 2);
    Matrix * avgs_actual = matrix_create(1, 2);
    Matrix * avgs_expected = matrix_create(1, 2);
    Matrix * stddevs_actual = matrix_create(1, 2);
    Matrix * stddevs_expected = matrix_create(1, 2);

    matrix->xs[0] =    5.0f;
    matrix->xs[1] =  -10.0f;
    matrix->xs[2] =    2.0f;
    matrix->xs[3] =  -15.0f;
    matrix->xs[4] =    8.0f;
    matrix->xs[5] =   -5.0f;

    expected->xs[0] =  0.0f;
    expected->xs[1] =  0.0f;
    expected->xs[2] = -1.224f;
    expected->xs[3] = -1.224f;
    expected->xs[4] =  1.224f;
    expected->xs[5] =  1.224f;

    avgs_expected->xs[0] =   5.0f;
    avgs_expected->xs[1] = -10.0f;

    stddevs_expected->xs[0] = 2.449f;
    stddevs_expected->xs[1] = 4.082f;

    matrix_stzdwn(matrix, avgs_actual, stddevs_actual, actual);
    {
        bool cond = matrix_testeq (actual, expected, 0.001f);
        cr_assert(cond, "expected a and b to be equal within the given tolerance (1)");
    }
    {
        bool cond = matrix_testeq (avgs_actual, avgs_expected, 0.001f);
        cr_assert(cond, "expected a and b to be equal within the given tolerance (2)");
    }
    {
        bool cond = matrix_testeq (stddevs_actual, stddevs_expected, 0.001f);
        cr_assert(cond, "expected a and b to be equal within the given tolerance (3)");
    }

    matrix_destroy(&stddevs_expected);
    matrix_destroy(&stddevs_actual);
    matrix_destroy(&avgs_expected);
    matrix_destroy(&avgs_actual);
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, stzrgt) {
    Matrix * matrix = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);
    Matrix * avgs_actual = matrix_create(2, 1);
    Matrix * avgs_expected = matrix_create(2, 1);
    Matrix * stddevs_actual = matrix_create(2, 1);
    Matrix * stddevs_expected = matrix_create(2, 1);

    matrix->xs[0] =    5.0f;
    matrix->xs[1] =    2.0f;
    matrix->xs[2] =    8.0f;
    matrix->xs[3] =  -10.0f;
    matrix->xs[4] =  -15.0f;
    matrix->xs[5] =   -5.0f;

    expected->xs[0] =  0.0f;
    expected->xs[1] = -1.224f;
    expected->xs[2] =  1.224f;
    expected->xs[3] =  0.0f;
    expected->xs[4] = -1.224f;
    expected->xs[5] =  1.224f;

    avgs_expected->xs[0] =   5.0f;
    avgs_expected->xs[1] = -10.0f;

    stddevs_expected->xs[0] = 2.449f;
    stddevs_expected->xs[1] = 4.082f;

    matrix_stzrgt(matrix, avgs_actual, stddevs_actual, actual);
    {
        bool cond = matrix_testeq (actual, expected, 0.001f);
        cr_assert(cond, "expected a and b to be equal within the given tolerance (1)");
    }
    {
        bool cond = matrix_testeq (avgs_actual, avgs_expected, 0.001f);
        cr_assert(cond, "expected a and b to be equal within the given tolerance (2)");
    }
    {
        bool cond = matrix_testeq (stddevs_actual, stddevs_expected, 0.001f);
        cr_assert(cond, "expected a and b to be equal within the given tolerance (3)");
    }

    matrix_destroy(&stddevs_expected);
    matrix_destroy(&stddevs_actual);
    matrix_destroy(&avgs_expected);
    matrix_destroy(&avgs_actual);
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, subsca) {
    Matrix * left = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);
    for (size_t i = 0; i < left->n; i++) {
        left->xs[i] = 10 + i;
        expected->xs[i] = i;
    }
    float right = 10.0f;
    matrix_subsca(left, right, actual);
    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&left);
}


Test(matrix, testeq) {
    size_t nra = 2;
    size_t nca = 3;
    Matrix * a = matrix_create(nra, nca);
    for (size_t ira = 0; ira < nra; ira++) {
        a->xs[ira] = ira;
    }
    size_t nrb = 2;
    size_t ncb = 3;
    Matrix * b = matrix_create(nrb, ncb);
    for (size_t irb = 0; irb < nrb; irb++) {
        b->xs[irb] = irb;
    }
    bool cond = matrix_testeq (a, b, 0.01f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&b);
    matrix_destroy(&a);
}


Test(matrix, transp) {
    Matrix * input = matrix_create(2, 3);
    input->xs[0] = 1.0f;
    input->xs[1] = 2.0f;
    input->xs[2] = 3.0f;
    input->xs[3] = 4.0f;
    input->xs[4] = 5.0f;
    input->xs[5] = 6.0f;

    Matrix * actual = matrix_create(3, 2);

    matrix_transp(input, actual);

    Matrix * expected = matrix_create(3, 2);
    expected->xs[0] = 1.0f;
    expected->xs[1] = 4.0f;
    expected->xs[2] = 2.0f;
    expected->xs[3] = 5.0f;
    expected->xs[4] = 3.0f;
    expected->xs[5] = 6.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "transpose failed");

    matrix_destroy(&input);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, varall) {
    Matrix * matrix = matrix_create(1, 8);
    matrix->xs[0] = 2.0f;
    matrix->xs[1] = 4.0f;
    matrix->xs[2] = 4.0f;
    matrix->xs[3] = 4.0f;
    matrix->xs[4] = 5.0f;
    matrix->xs[5] = 5.0f;
    matrix->xs[6] = 7.0f;
    matrix->xs[7] = 9.0f;
    float actual = matrix_varall(matrix);
    float expected = 4.0f;
    float tolerance = 0.001f;
    float lower;
    float upper;
    derive_limits_of_acceptability(expected, tolerance, &lower, &upper);
    cr_assert(lower < actual && actual < upper, "expected actual to be equal to %.1f within the given tolerance (was %.1f)", expected, actual);
    matrix_destroy(&matrix);
}


Test(matrix, vardwn) {
    Matrix * matrix = matrix_create(3, 3);
    Matrix * actual = matrix_create(1, 3);
    Matrix * expected = matrix_create(1, 3);

    matrix->xs[0] = 0.0f;
    matrix->xs[1] = 1.0f;
    matrix->xs[2] = 2.0f;
    matrix->xs[3] = 3.0f;
    matrix->xs[4] = 5.0f;
    matrix->xs[5] = 7.0f;
    matrix->xs[6] = 8.0f;
    matrix->xs[7] = 11.0f;
    matrix->xs[8] = 14.0f;

    expected->xs[0] = 10.889;
    expected->xs[1] = 16.889;
    expected->xs[2] = 24.222;

    matrix_vardwn(matrix, actual);

    bool cond = matrix_testeq(actual, expected, 0.01);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&matrix);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, varrgt) {
    Matrix * matrix = matrix_create(3, 3);
    Matrix * actual = matrix_create(3, 1);
    Matrix * expected = matrix_create(3, 1);

    matrix->xs[0] = 0.0f;
    matrix->xs[1] = 1.0f;
    matrix->xs[2] = 2.0f;
    matrix->xs[3] = 3.0f;
    matrix->xs[4] = 5.0f;
    matrix->xs[5] = 7.0f;
    matrix->xs[6] = 8.0f;
    matrix->xs[7] = 11.0f;
    matrix->xs[8] = 14.0f;

    expected->xs[0] = 0.667;
    expected->xs[1] = 2.667;
    expected->xs[2] = 6.000;

    matrix_varrgt(matrix, actual);

    bool cond = matrix_testeq(actual, expected, 0.01);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&matrix);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, vstack) {
    Matrix * top = matrix_create(2, 3);
    Matrix * bottom = matrix_create(2, 3);
    Matrix * actual = matrix_create(4, 3);
    Matrix * expected = matrix_create(4, 3);

    top->xs[0] = 1.0f;
    top->xs[1] = 2.0f;
    top->xs[2] = 3.0f;
    top->xs[3] = 4.0f;
    top->xs[4] = 5.0f;
    top->xs[5] = 6.0f;

    bottom->xs[0] =  7.0f;
    bottom->xs[1] =  8.0f;
    bottom->xs[2] =  9.0f;
    bottom->xs[3] = 10.0f;
    bottom->xs[4] = 11.0f;
    bottom->xs[5] = 12.0f;

    matrix_vstack(top, bottom, actual);

    expected->xs[0] =   1.0f;
    expected->xs[1] =   2.0f;
    expected->xs[2] =   3.0f;
    expected->xs[3] =   4.0f;
    expected->xs[4] =   5.0f;
    expected->xs[5] =   6.0f;
    expected->xs[6] =   7.0f;
    expected->xs[7] =   8.0f;
    expected->xs[8] =   9.0f;
    expected->xs[9] =  10.0f;
    expected->xs[10] = 11.0f;
    expected->xs[11] = 12.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "vstack failed");

    matrix_destroy(&top);
    matrix_destroy(&bottom);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, zeros) {
    Matrix * actual = matrix_create(2, 3);
    actual->xs[0] = 1.0f;
    actual->xs[1] = 2.0f;
    actual->xs[2] = 3.0f;
    actual->xs[3] = 4.0f;
    actual->xs[4] = 5.0f;
    actual->xs[5] = 6.0f;

    matrix_zeros(actual);

    Matrix * expected = matrix_create(2, 3);
    expected->xs[0] = 0.0f;
    expected->xs[1] = 0.0f;
    expected->xs[2] = 0.0f;
    expected->xs[3] = 0.0f;
    expected->xs[4] = 0.0f;
    expected->xs[5] = 0.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "zeros failed");

    matrix_destroy(&expected);
    matrix_destroy(&actual);
}
