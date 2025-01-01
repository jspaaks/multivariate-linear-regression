#include "matrix/matrix.h"
#include <criterion/criterion.h>

void derive_limits_of_acceptability(float expected, float tolerance, float * lower, float * upper);

void derive_limits_of_acceptability(float expected, float tolerance, float * lower, float * upper) {
    *lower = expected - tolerance;
    *upper = expected + tolerance;
}


Test(matrix, accall) {
    Matrix * matrix = matrix_create(3, 2);
    matrix->vals[0] =   2.0f;
    matrix->vals[1] =   4.0f;
    matrix->vals[2] =  12.0f;
    matrix->vals[3] =  22.0f;
    matrix->vals[4] = 452.0f;
    matrix->vals[5] =  -4.0f;
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
        matrix->vals[i] = i;
    }

    expected->vals[0] = 2.0f;
    expected->vals[1] = 4.0f;

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
        matrix->vals[i] = i;
    }

    expected->vals[0] = 1.0f;
    expected->vals[1] = 5.0f;

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
        left->vals[i] = i;
        expected->vals[i] = 10 + i;
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
        matrix->vals[i] = i;
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
        matrix->vals[i] = i;
    }

    for (size_t i = 0; i < expected->n; i++) {
        expected->vals[i] = i + 1.5f;
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
        matrix->vals[i] = i;
    }

    for (size_t i = 0; i < expected->n; i++) {
        expected->vals[i] = 2 * i + 0.5f;
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
        matrix->vals[i] = i;
    }

    for (size_t ir = 0; ir < expected->nr; ir++) {
        for (size_t ic = 0; ic < expected->nc; ic++) {
            size_t i = ir * expected->nc + ic;
            expected->vals[i] = ic;
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
        matrix->vals[i] = i;
    }

    for (size_t ir = 0; ir < expected->nr; ir++) {
        for (size_t ic = 0; ic < expected->nc; ic++) {
            size_t i = ir * expected->nc + ic;
            expected->vals[i] = ir;
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
        matrix->vals[i] = i;
        expected->vals[i] = i / factor;
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
        a->vals[i] = i;
    }
    for (size_t i = 0; i < b->n; i++) {
        b->vals[i] = i + 10;
    }
    matrix_dotpro(a, b, c);
    const float expected[8] = {  50.0f,  53.0f,  56.0f,  59.0f, 176.0f, 188.0f, 200.0f, 212.0f};
    const size_t n = c->nr * c->nc;
    for (size_t i = 0; i < n; i++) {
        const float lower = expected[i] - 0.01f;
        const float upper = expected[i] + 0.01f;
        const float actual = c->vals[i];
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
        left->vals[i] = i;
        right->vals[i] = 10;
        expected->vals[i] = 10 + i;
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
        left->vals[i] = 10 + i;
        right->vals[i] = i;
        expected->vals[i] = 10;
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
            left->vals[i] = ic + 1;
            right->vals[i] = ir + 1;
            expected->vals[i] = (ic + 1) * (ir + 1);
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

    left->vals[0] = 1.0f;
    left->vals[1] = 2.0f;
    left->vals[2] = 3.0f;
    left->vals[3] = 7.0f;
    left->vals[4] = 8.0f;
    left->vals[5] = 9.0f;

    right->vals[0] =  4.0f;
    right->vals[1] =  5.0f;
    right->vals[2] =  6.0f;
    right->vals[3] = 10.0f;
    right->vals[4] = 11.0f;
    right->vals[5] = 12.0f;

    matrix_hstack(left, right, actual);

    expected->vals[0] =   1.0f;
    expected->vals[1] =   2.0f;
    expected->vals[2] =   3.0f;
    expected->vals[3] =   4.0f;
    expected->vals[4] =   5.0f;
    expected->vals[5] =   6.0f;
    expected->vals[6] =   7.0f;
    expected->vals[7] =   8.0f;
    expected->vals[8] =   9.0f;
    expected->vals[9] =  10.0f;
    expected->vals[10] = 11.0f;
    expected->vals[11] = 12.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "vstack failed");

    matrix_destroy(&left);
    matrix_destroy(&right);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, maxall) {
    Matrix * matrix = matrix_create(3, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
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
        matrix->vals[i] = i;
    }
    expected->vals[0] = 4.0f;
    expected->vals[1] = 5.0f;
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
        matrix->vals[i] = i;
    }
    expected->vals[0] = 2.0f;
    expected->vals[1] = 5.0f;
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
        matrix->vals[i] = i;
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
        matrix->vals[i] = i;
    }
    expected->vals[0] = 0.0f;
    expected->vals[1] = 1.0f;
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
        matrix->vals[i] = i;
    }
    expected->vals[0] = 0.0f;
    expected->vals[1] = 3.0f;
    matrix_minrgt(matrix, actual);
    bool cond = matrix_testeq(actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}



Test(matrix, ones) {
    Matrix * actual = matrix_create(2, 3);
    actual->vals[0] = 1.0f;
    actual->vals[1] = 2.0f;
    actual->vals[2] = 3.0f;
    actual->vals[3] = 4.0f;
    actual->vals[4] = 5.0f;
    actual->vals[5] = 6.0f;

    matrix_ones(actual);

    Matrix * expected = matrix_create(2, 3);
    expected->vals[0] = 1.0f;
    expected->vals[1] = 1.0f;
    expected->vals[2] = 1.0f;
    expected->vals[3] = 1.0f;
    expected->vals[4] = 1.0f;
    expected->vals[5] = 1.0f;

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
        matrix->vals[i] = i;
        expected->vals[i] = i * factor;
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
    matrix->vals[0] = 2.0f;
    matrix->vals[1] = 4.0f;
    matrix->vals[2] = 4.0f;
    matrix->vals[3] = 4.0f;
    matrix->vals[4] = 5.0f;
    matrix->vals[5] = 5.0f;
    matrix->vals[6] = 7.0f;
    matrix->vals[7] = 9.0f;
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

    matrix->vals[0] = 0.0f;
    matrix->vals[1] = 1.0f;
    matrix->vals[2] = 2.0f;
    matrix->vals[3] = 3.0f;
    matrix->vals[4] = 5.0f;
    matrix->vals[5] = 7.0f;
    matrix->vals[6] = 8.0f;
    matrix->vals[7] = 11.0f;
    matrix->vals[8] = 14.0f;

    expected->vals[0] = 3.300;
    expected->vals[1] = 4.110;
    expected->vals[2] = 4.922;

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

    matrix->vals[0] = 0.0f;
    matrix->vals[1] = 1.0f;
    matrix->vals[2] = 2.0f;
    matrix->vals[3] = 3.0f;
    matrix->vals[4] = 5.0f;
    matrix->vals[5] = 7.0f;
    matrix->vals[6] = 8.0f;
    matrix->vals[7] = 11.0f;
    matrix->vals[8] = 14.0f;

    expected->vals[0] = 0.816;
    expected->vals[1] = 1.633;
    expected->vals[2] = 2.449;

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

    matrix->vals[0] =    5.0f;
    matrix->vals[1] =  -10.0f;
    matrix->vals[2] =    2.0f;
    matrix->vals[3] =  -15.0f;
    matrix->vals[4] =    8.0f;
    matrix->vals[5] =   -5.0f;

    expected->vals[0] =  0.0f;
    expected->vals[1] =  0.0f;
    expected->vals[2] = -1.224f;
    expected->vals[3] = -1.224f;
    expected->vals[4] =  1.224f;
    expected->vals[5] =  1.224f;

    avgs_expected->vals[0] =   5.0f;
    avgs_expected->vals[1] = -10.0f;

    stddevs_expected->vals[0] = 2.449f;
    stddevs_expected->vals[1] = 4.082f;

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

    matrix->vals[0] =    5.0f;
    matrix->vals[1] =    2.0f;
    matrix->vals[2] =    8.0f;
    matrix->vals[3] =  -10.0f;
    matrix->vals[4] =  -15.0f;
    matrix->vals[5] =   -5.0f;

    expected->vals[0] =  0.0f;
    expected->vals[1] = -1.224f;
    expected->vals[2] =  1.224f;
    expected->vals[3] =  0.0f;
    expected->vals[4] = -1.224f;
    expected->vals[5] =  1.224f;

    avgs_expected->vals[0] =   5.0f;
    avgs_expected->vals[1] = -10.0f;

    stddevs_expected->vals[0] = 2.449f;
    stddevs_expected->vals[1] = 4.082f;

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
        left->vals[i] = 10 + i;
        expected->vals[i] = i;
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
        a->vals[ira] = ira;
    }
    size_t nrb = 2;
    size_t ncb = 3;
    Matrix * b = matrix_create(nrb, ncb);
    for (size_t irb = 0; irb < nrb; irb++) {
        b->vals[irb] = irb;
    }
    bool cond = matrix_testeq (a, b, 0.01f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&b);
    matrix_destroy(&a);
}


Test(matrix, transp) {
    Matrix * input = matrix_create(2, 3);
    input->vals[0] = 1.0f;
    input->vals[1] = 2.0f;
    input->vals[2] = 3.0f;
    input->vals[3] = 4.0f;
    input->vals[4] = 5.0f;
    input->vals[5] = 6.0f;

    Matrix * actual = matrix_create(3, 2);

    matrix_transp(input, actual);

    Matrix * expected = matrix_create(3, 2);
    expected->vals[0] = 1.0f;
    expected->vals[1] = 4.0f;
    expected->vals[2] = 2.0f;
    expected->vals[3] = 5.0f;
    expected->vals[4] = 3.0f;
    expected->vals[5] = 6.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "transpose failed");

    matrix_destroy(&input);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, varall) {
    Matrix * matrix = matrix_create(1, 8);
    matrix->vals[0] = 2.0f;
    matrix->vals[1] = 4.0f;
    matrix->vals[2] = 4.0f;
    matrix->vals[3] = 4.0f;
    matrix->vals[4] = 5.0f;
    matrix->vals[5] = 5.0f;
    matrix->vals[6] = 7.0f;
    matrix->vals[7] = 9.0f;
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

    matrix->vals[0] = 0.0f;
    matrix->vals[1] = 1.0f;
    matrix->vals[2] = 2.0f;
    matrix->vals[3] = 3.0f;
    matrix->vals[4] = 5.0f;
    matrix->vals[5] = 7.0f;
    matrix->vals[6] = 8.0f;
    matrix->vals[7] = 11.0f;
    matrix->vals[8] = 14.0f;

    expected->vals[0] = 10.889;
    expected->vals[1] = 16.889;
    expected->vals[2] = 24.222;

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

    matrix->vals[0] = 0.0f;
    matrix->vals[1] = 1.0f;
    matrix->vals[2] = 2.0f;
    matrix->vals[3] = 3.0f;
    matrix->vals[4] = 5.0f;
    matrix->vals[5] = 7.0f;
    matrix->vals[6] = 8.0f;
    matrix->vals[7] = 11.0f;
    matrix->vals[8] = 14.0f;

    expected->vals[0] = 0.667;
    expected->vals[1] = 2.667;
    expected->vals[2] = 6.000;

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

    top->vals[0] = 1.0f;
    top->vals[1] = 2.0f;
    top->vals[2] = 3.0f;
    top->vals[3] = 4.0f;
    top->vals[4] = 5.0f;
    top->vals[5] = 6.0f;

    bottom->vals[0] =  7.0f;
    bottom->vals[1] =  8.0f;
    bottom->vals[2] =  9.0f;
    bottom->vals[3] = 10.0f;
    bottom->vals[4] = 11.0f;
    bottom->vals[5] = 12.0f;

    matrix_vstack(top, bottom, actual);

    expected->vals[0] =   1.0f;
    expected->vals[1] =   2.0f;
    expected->vals[2] =   3.0f;
    expected->vals[3] =   4.0f;
    expected->vals[4] =   5.0f;
    expected->vals[5] =   6.0f;
    expected->vals[6] =   7.0f;
    expected->vals[7] =   8.0f;
    expected->vals[8] =   9.0f;
    expected->vals[9] =  10.0f;
    expected->vals[10] = 11.0f;
    expected->vals[11] = 12.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "vstack failed");

    matrix_destroy(&top);
    matrix_destroy(&bottom);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}


Test(matrix, zeros) {
    Matrix * actual = matrix_create(2, 3);
    actual->vals[0] = 1.0f;
    actual->vals[1] = 2.0f;
    actual->vals[2] = 3.0f;
    actual->vals[3] = 4.0f;
    actual->vals[4] = 5.0f;
    actual->vals[5] = 6.0f;

    matrix_zeros(actual);

    Matrix * expected = matrix_create(2, 3);
    expected->vals[0] = 0.0f;
    expected->vals[1] = 0.0f;
    expected->vals[2] = 0.0f;
    expected->vals[3] = 0.0f;
    expected->vals[4] = 0.0f;
    expected->vals[5] = 0.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "zeros failed");

    matrix_destroy(&expected);
    matrix_destroy(&actual);
}
