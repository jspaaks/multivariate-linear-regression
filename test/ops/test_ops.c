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
