#include "matrix/matrix.h"
#include <criterion/criterion.h>


Test(matrix, dotproduct) {
    Matrix * a = matrix_create(2, 3);
    Matrix * b = matrix_create(3, 4);
    Matrix * c = matrix_create(2, 4);
    for (size_t i = 0; i < a->nr * a->nc; i++) {
        a->vals[i] = i;
    }
    for (size_t i = 0; i < b->nr * b->nc; i++) {
        b->vals[i] = i + 10;
    }
    matrix_dotproduct(a, b, c);
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


Test(matrix, map_eq) {
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
    bool cond = matrix_map_eq (a, b, 0.01f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&b);
    matrix_destroy(&a);
}


Test(matrix, map_af) {
    Matrix * in = matrix_create(2, 3);
    Matrix * out = matrix_create(2, 3);

    in->vals[0] = 3.2f;
    in->vals[1] = 1.3f;
    in->vals[2] = 2.2f;
    in->vals[3] = 5.1f;
    in->vals[4] = 4.9f;
    in->vals[5] = 2.5f;

    out->vals[0] = 3.2f;
    out->vals[1] = 1.3f;
    out->vals[2] = 2.2f;
    out->vals[3] = 5.1f;
    out->vals[4] = 4.9f;
    out->vals[5] = 2.5f;

    matrix_map_af(afuns_passthrough, in, out);
    bool cond = matrix_map_eq(in, out, 0.01f);
    cr_assert(cond);

    matrix_destroy(&in);
    matrix_destroy(&out);
}


Test(matrix, transpose) {
    Matrix * input = matrix_create(2, 3);
    input->vals[0] = 1.0f;
    input->vals[1] = 2.0f;
    input->vals[2] = 3.0f;
    input->vals[3] = 4.0f;
    input->vals[4] = 5.0f;
    input->vals[5] = 6.0f;

    Matrix * actual = matrix_create(3, 2);

    matrix_transpose(input, actual);

    Matrix * expected = matrix_create(3, 2);
    expected->vals[0] = 1.0f;
    expected->vals[1] = 4.0f;
    expected->vals[2] = 2.0f;
    expected->vals[3] = 5.0f;
    expected->vals[4] = 3.0f;
    expected->vals[5] = 6.0f;

    bool cond = matrix_map_eq(actual, expected, 0.01f);
    cr_assert(cond, "transpose failed");

    matrix_destroy(&input);
    matrix_destroy(&actual);
    matrix_destroy(&expected);
}
