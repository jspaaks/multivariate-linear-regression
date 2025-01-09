#include "matrix/matrix.h"
#include "ops/ops.h"
#include <criterion/criterion.h>


Test(ops, svm) {
    Matrix * scores = matrix_create(3, 3);
    scores->xs[0] = 3.2f;
    scores->xs[1] = 1.3f;
    scores->xs[2] = 2.2f;
    scores->xs[3] = 5.1f;
    scores->xs[4] = 4.9f;
    scores->xs[5] = 2.5f;
    scores->xs[6] = -1.7f;
    scores->xs[7] = 2.0f;
    scores->xs[8] = -3.1f;


    Matrix * labels = matrix_create(1, 3);
    labels->xs[0] = 0.0f;
    labels->xs[1] = 1.0f;
    labels->xs[2] = 2.0f;

    float expected = (2.9f + 0.0f + 12.9f) / 3;
    float lower = expected - 0.001f;
    float upper = expected + 0.001f;
    float actual = 0.0f;
    ops_svm(scores, labels, &actual);

    cr_assert(lower < actual && actual < upper, "actual was %.6f", actual);

    matrix_destroy(&scores);
    matrix_destroy(&labels);
}
