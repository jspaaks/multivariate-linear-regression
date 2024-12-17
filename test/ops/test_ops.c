#include "data/data.h"
#include "ops/ops.h"
#include <criterion/criterion.h>


Test(ops, dot_product) {
    Matrix * a = data_create(2, 3);
    Matrix * b = data_create(3, 4);
    Matrix * c = data_create(2, 4);
    for (size_t i = 0; i < a->nr * a->nc; i++) {
        a->vals[i] = i;
    }
    for (size_t i = 0; i < b->nr * b->nc; i++) {
        b->vals[i] = i + 10;
    }
    ops_dot_product(a, b, c);
    const float expected[8] = {  50.0f,  53.0f,  56.0f,  59.0f, 176.0f, 188.0f, 200.0f, 212.0f};
    const size_t n = c->nr * c->nc;
    for (size_t i = 0; i < n; i++) {
        const float lower = expected[i] - 0.01f;
        const float upper = expected[i] + 0.01f;
        const float actual = c->vals[i];
        cr_assert(lower < actual && actual < upper, "actual was %.6f", actual);
    }
    data_destroy(&c);
    data_destroy(&b);
    data_destroy(&a);
}


Test(ops, eq) {
    size_t nra = 2;
    size_t nca = 3;
    Matrix * a = data_create(nra, nca);
    for (size_t ira = 0; ira < nra; ira++) {
        a->vals[ira] = ira;
    }
    size_t nrb = 2;
    size_t ncb = 3;
    Matrix * b = data_create(nrb, ncb);
    for (size_t irb = 0; irb < nrb; irb++) {
        b->vals[irb] = irb;
    }
    bool cond = ops_map_eq (a, b, 0.01f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    data_destroy(&b);
    data_destroy(&a);
}


Test(ops, map) {
    Matrix * in = data_create(2, 3);
    Matrix * out = data_create(2, 3);
    Matrix * expected = data_create(2, 3);

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

    ops_map_af(afuns_passthrough, in, out);
    bool cond = ops_map_eq(in, out, 0.01f);
    cr_assert(cond);

    data_destroy(&in);
    data_destroy(&out);
    data_destroy(&expected);
}


Test(ops, svm) {
    Matrix * scores = data_create(3, 3);
    scores->vals[0] = 3.2f;
    scores->vals[1] = 1.3f;
    scores->vals[2] = 2.2f;
    scores->vals[3] = 5.1f;
    scores->vals[4] = 4.9f;
    scores->vals[5] = 2.5f;
    scores->vals[6] = -1.7f;
    scores->vals[7] = 2.0f;
    scores->vals[8] = -3.1f;


    Matrix * labels = data_create(1, 3);
    labels->vals[0] = 0.0f;
    labels->vals[1] = 1.0f;
    labels->vals[2] = 2.0f;

    float expected = (2.9f + 0.0f + 12.9f) / 3;
    float lower = expected - 0.001f;
    float upper = expected + 0.001f;
    float actual = 0.0f;
    ops_svm(scores, labels, &actual);

    cr_assert(lower < actual && actual < upper, "actual was %.6f", actual);
}
