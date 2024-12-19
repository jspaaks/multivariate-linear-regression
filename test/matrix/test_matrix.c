#include "matrix/matrix.h"
#include <criterion/criterion.h>


Test(matrix, acc_per_col) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * actual = matrix_create(1, 2);
    Matrix * expected = matrix_create(1, 2);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }

    expected->vals[0] = 2.0f;
    expected->vals[1] = 4.0f;

    matrix_acc_per_col(matrix, actual);

    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, acc_per_row) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * actual = matrix_create(2, 1);
    Matrix * expected = matrix_create(2, 1);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }

    expected->vals[0] = 1.0f;
    expected->vals[1] = 5.0f;

    matrix_acc_per_row(matrix, actual);

    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, add) {
    Matrix * left = matrix_create(2, 2);
    Matrix * right = matrix_create(2, 2);
    Matrix * actual = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);

    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = i;
        right->vals[i] = 10;
        expected->vals[i] = 10 + i;
    }
    matrix_add(left, right, actual);
    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, add_) {
    Matrix * left = matrix_create(2, 2);
    Matrix * right = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);

    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = i;
        right->vals[i] = 10;
        expected->vals[i] = 10 + i;
    }
    matrix_add_(left, right);
    bool cond = matrix_map_eq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, add_scalar) {
    Matrix * left = matrix_create(2, 2);
    float right = 10.0f;
    Matrix * actual = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);

    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = i;
        expected->vals[i] = 10 + i;
    }
    matrix_add_scalar(left, right, actual);
    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&left);
}


Test(matrix, add_scalar_) {
    Matrix * left = matrix_create(2, 2);
    float right = 10.0f;
    Matrix * expected = matrix_create(2, 2);

    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = i;
        expected->vals[i] = 10 + i;
    }
    matrix_add_scalar_(left, right);
    bool cond = matrix_map_eq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&left);
}


Test(matrix, avg) {
    Matrix * matrix = matrix_create(2, 3);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }
    float expected = 2.5f;
    float tolerance = 0.0001f;
    float lower = expected - tolerance;
    float upper = expected + tolerance;
    float actual = matrix_avg(matrix);
    cr_assert(lower < actual && actual < upper, "expected actual to be equal to %.1f within the given tolerance (was %.1f)", expected, actual);
    matrix_destroy(&matrix);
}


Test(matrix, avg_per_col) {
    Matrix * matrix = matrix_create(2, 3);
    Matrix * actual = matrix_create(1, 3);
    Matrix * expected = matrix_create(1, 3);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }

    for (size_t i = 0; i < expected->n; i++) {
        expected->vals[i] = i + 1.5f;
    }

    matrix_avg_per_col(matrix, actual);

    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, avg_per_row) {
    Matrix * matrix = matrix_create(3, 2);
    Matrix * actual = matrix_create(3, 1);
    Matrix * expected = matrix_create(3, 1);

    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }

    for (size_t i = 0; i < expected->n; i++) {
        expected->vals[i] = 2 * i + 0.5f;
    }

    matrix_avg_per_row(matrix, actual);

    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, broadcast_down) {
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

    matrix_broadcast_down(matrix, actual);

    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, broadcast_right) {
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

    matrix_broadcast_right(matrix, actual);

    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, divide_scalar) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * actual = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);
    float factor = 10.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
        expected->vals[i] = i / factor;
    }
    matrix_divide_scalar(matrix, factor, actual);
    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, divide_scalar_) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);
    float factor = 10.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
        expected->vals[i] = i / factor;
    }
    matrix_divide_scalar_(matrix, factor);
    bool cond = matrix_map_eq (matrix, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&matrix);
}


Test(matrix, dotproduct) {
    Matrix * a = matrix_create(2, 3);
    Matrix * b = matrix_create(3, 4);
    Matrix * c = matrix_create(2, 4);
    for (size_t i = 0; i < a->n; i++) {
        a->vals[i] = i;
    }
    for (size_t i = 0; i < b->n; i++) {
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


Test(matrix, hadamardproduct) {
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
    matrix_hadamardproduct(left, right, actual);
    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, hadamardproduct_) {
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
    matrix_hadamardproduct_(left, right);
    bool cond = matrix_map_eq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, highest) {
    Matrix * matrix = matrix_create(3, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }
    float actual = matrix_highest(matrix);
    float expected = 5.0f;
    float tolerance = 0.001f;
    float lower = expected - tolerance;
    float upper = expected + tolerance;
    cr_assert(lower < actual && actual < upper, "expected actual to be equal to %.1f within the given tolerance (was %.1f)", expected, actual);
    matrix_destroy(&matrix);
}


Test(matrix, highest_per_col) {
    Matrix * matrix = matrix_create(3, 2);
    Matrix * actual = matrix_create(1, 2);
    Matrix * expected = matrix_create(1, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }
    expected->vals[0] = 4.0f;
    expected->vals[1] = 5.0f;
    matrix_highest_per_col(matrix, actual);
    bool cond = matrix_map_eq(actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, highest_per_row) {
    Matrix * matrix = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 1);
    Matrix * expected = matrix_create(2, 1);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }
    expected->vals[0] = 2.0f;
    expected->vals[1] = 5.0f;
    matrix_highest_per_row(matrix, actual);
    bool cond = matrix_map_eq(actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, lowest) {
    Matrix * matrix = matrix_create(3, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }
    float actual = matrix_lowest(matrix);
    float expected = 0.0f;
    float tolerance = 0.001f;
    float lower = expected - tolerance;
    float upper = expected + tolerance;
    cr_assert(lower < actual && actual < upper, "expected actual to be equal to %.1f within the given tolerance (was %.1f)", expected, actual);
    matrix_destroy(&matrix);
}


Test(matrix, lowest_per_col) {
    Matrix * matrix = matrix_create(3, 2);
    Matrix * actual = matrix_create(1, 2);
    Matrix * expected = matrix_create(1, 2);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }
    expected->vals[0] = 0.0f;
    expected->vals[1] = 1.0f;
    matrix_lowest_per_col(matrix, actual);
    bool cond = matrix_map_eq(actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, lowest_per_row) {
    Matrix * matrix = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 1);
    Matrix * expected = matrix_create(2, 1);
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
    }
    expected->vals[0] = 0.0f;
    expected->vals[1] = 3.0f;
    matrix_lowest_per_row(matrix, actual);
    bool cond = matrix_map_eq(actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
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


Test(matrix, multiply_scalar) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * actual = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);
    float factor = 10.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
        expected->vals[i] = i * factor;
    }
    matrix_multiply_scalar(matrix, factor, actual);
    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&matrix);
}


Test(matrix, multiply_scalar_) {
    Matrix * matrix = matrix_create(2, 2);
    Matrix * expected = matrix_create(2, 2);
    float factor = 10.0f;
    for (size_t i = 0; i < matrix->n; i++) {
        matrix->vals[i] = i;
        expected->vals[i] = i * factor;
    }
    matrix_multiply_scalar_(matrix, factor);
    bool cond = matrix_map_eq (matrix, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&matrix);
}


Test(matrix, subtract) {
    Matrix * left = matrix_create(2, 3);
    Matrix * right = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);
    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = 10 + i;
        right->vals[i] = i;
        expected->vals[i] = 10;
    }
    matrix_subtract(left, right, actual);
    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, subtract_) {
    Matrix * left = matrix_create(2, 3);
    Matrix * right = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);
    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = 10 + i;
        right->vals[i] = i;
        expected->vals[i] = 10;
    }
    matrix_subtract_(left, right);
    bool cond = matrix_map_eq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&right);
    matrix_destroy(&left);
}


Test(matrix, subtract_scalar) {
    Matrix * left = matrix_create(2, 3);
    Matrix * actual = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);
    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = 10 + i;
        expected->vals[i] = i;
    }
    float right = 10.0f;
    matrix_subtract_scalar(left, right, actual);
    bool cond = matrix_map_eq (actual, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&actual);
    matrix_destroy(&left);
}


Test(matrix, subtract_scalar_) {
    Matrix * left = matrix_create(2, 3);
    Matrix * expected = matrix_create(2, 3);
    for (size_t i = 0; i < left->n; i++) {
        left->vals[i] = 10 + i;
        expected->vals[i] = i;
    }
    float right = 10.0f;
    matrix_subtract_scalar_(left, right);
    bool cond = matrix_map_eq (left, expected, 0.0001f);
    cr_assert(cond, "expected a and b to be equal within the given tolerance");
    matrix_destroy(&expected);
    matrix_destroy(&left);
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

    bool cond = matrix_map_eq(actual, expected, 0.01f);
    cr_assert(cond, "zero failed");

    matrix_destroy(&expected);
    matrix_destroy(&actual);
}
