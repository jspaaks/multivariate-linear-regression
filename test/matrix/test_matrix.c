#include "matrix/matrix.h"
#include <criterion/criterion.h>


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


Test(matrix, addsca_) {
    Matrix * left = matrix_create(2, 2);
    float right = 10.0f;
    Matrix * expected = matrix_create(2, 2);

    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = i;
        expected->vals[i] = 10 + i;
    }
    matrix_addsca_(left, right);
    bool cond = matrix_testeq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&left);
}


Test(matrix, avgall) {
    Matrix * matrix = matrix_create(2, 3);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }
    float expected = 2.5f;
    float tolerance = 0.0001f;
    float lower = expected - tolerance;
    float upper = expected + tolerance;
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


Test(matrix, bcastd) {
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

    matrix_bcastd(matrix, actual);

    bool cond = matrix_testeq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, bcastr) {
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

    matrix_bcastr(matrix, actual);

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


Test(matrix, divsca_) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);
    float factor = 10.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
        expected->vals[i] = i / factor;
    }
    matrix_divsca_(matrix, factor);
    bool cond = matrix_testeq (matrix, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
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


Test(matrix, ebeadd_) {
    Matrix * left = matrix_create(2, 2);
    Matrix * right = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);

    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = i;
        right->vals[i] = 10;
        expected->vals[i] = 10 + i;
    }
    matrix_ebeadd_(left, right);
    bool cond = matrix_testeq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, ebediv) {
}


Test(matrix, ebemap) {
}


Test(matrix, ebemap_) {
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


Test(matrix, ebesub_) {
    Matrix * left = matrix_create(2, 3);
    Matrix * right = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);
    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = 10 + i;
        right->vals[i] = i;
        expected->vals[i] = 10;
    }
    matrix_ebesub_(left, right);
    bool cond = matrix_testeq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
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


Test(matrix, hadpro_) {
    Matrix * left = matrix_create(2, 3);
    Matrix * right = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);

    for (size_t ir = 0; ir < left->nr; ir++) {
        for (size_t ic = 0; ic < left->nc; ic++) {
            size_t i = ir * left->nc + ic;
            left->vals[i] = ic + 1;
            right->vals[i] = ir + 1;
            expected->vals[i] = (ic + 1) * (ir + 1);
        }
    }
    matrix_hadpro_(left, right);
    bool cond = matrix_testeq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, maxall) {
    Matrix * matrix = matrix_create(3, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }
    float actual = matrix_maxall(matrix);
    float expected = 5.0f;
    float tolerance = 0.001f;
    float lower = expected - tolerance;
    float upper = expected + tolerance;
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
    float lower = expected - tolerance;
    float upper = expected + tolerance;
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


Test(matrix, scapro_) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);
    float factor = 10.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
        expected->vals[i] = i * factor;
    }
    matrix_scapro_(matrix, factor);
    bool cond = matrix_testeq (matrix, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&matrix);
}


Test(matrix, stzdwn_) {
    Matrix * matrix = matrix_create(3, 2);
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
    expected->vals[2] = -0.707f;
    expected->vals[3] = -0.707f;
    expected->vals[4] =  0.707f;
    expected->vals[5] =  0.707f;

    avgs_expected->vals[0] =   5.0f;
    avgs_expected->vals[1] = -10.0f;

    stddevs_expected->vals[0] = 4.242f;
    stddevs_expected->vals[1] = 7.071f;

    matrix_stzdwn_(matrix, avgs_actual, stddevs_actual);
    {
        bool cond = matrix_testeq (matrix, expected, 0.001f);
        cr_assert(cond, "expected a and b to be equal within the given tolerance");
    }
    {
        bool cond = matrix_testeq (avgs_actual, avgs_expected, 0.001f);
        cr_assert(cond, "expected a and b to be equal within the given tolerance");
    }
    {
        bool cond = matrix_testeq (stddevs_actual, stddevs_expected, 0.001f);
        cr_assert(cond, "expected a and b to be equal within the given tolerance");
    }

    matrix_destroy(&stddevs_expected);
    matrix_destroy(&stddevs_actual);
    matrix_destroy(&avgs_expected);
    matrix_destroy(&avgs_actual);
    matrix_destroy(&expected);
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


Test(matrix, subsca_) {
    Matrix * left = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);
    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = 10 + i;
        expected->vals[i] = i;
    }
    float right = 10.0f;
    matrix_subsca_(left, right);
    bool cond = matrix_testeq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
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


Test(matrix, zero_) {
    Matrix * actual = matrix_create(2, 3);
    actual->vals[0] = 1.0f;
    actual->vals[1] = 2.0f;
    actual->vals[2] = 3.0f;
    actual->vals[3] = 4.0f;
    actual->vals[4] = 5.0f;
    actual->vals[5] = 6.0f;

    matrix_zero_(actual);

    Matrix * expected = matrix_create(2, 3);
    expected->vals[0] = 0.0f;
    expected->vals[1] = 0.0f;
    expected->vals[2] = 0.0f;
    expected->vals[3] = 0.0f;
    expected->vals[4] = 0.0f;
    expected->vals[5] = 0.0f;

    bool cond = matrix_testeq(actual, expected, 0.01f);
    cr_assert(cond, "zero failed");

    matrix_destroy(&expected);
    matrix_destroy(&actual);
}
