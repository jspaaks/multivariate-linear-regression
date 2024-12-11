#include "lfuns/lfuns.h"
#include <criterion/criterion.h>

Test(ofuns, svm_cat){
    // https://www.youtube.com/watch?v=h7iBpEHGVNc&t=928s
    constexpr size_t n = 3;
    float scores[n] = {3.2, 5.1, -1.7};
    size_t m = 0;
    float actual = lfuns_svm(n, &scores[0], m);
    float expected = 2.9;
    float eps = 0.0001;
    float lower = expected - eps;
    float upper = expected + eps;
    cr_assert(lower < actual && actual < upper);
}

Test(ofuns, svm_car){
    // https://www.youtube.com/watch?v=h7iBpEHGVNc&t=1000s
    constexpr size_t n = 3;
    float scores[n] = {1.3, 4.9, 2.0};
    size_t m = 1;
    float actual = lfuns_svm(n, &scores[0], m);
    float expected = 0.0;
    float eps = 0.0001;
    float lower = expected - eps;
    float upper = expected + eps;
    cr_assert(lower < actual && actual < upper);
}

Test(ofuns, svm_frog){
    // https://www.youtube.com/watch?v=h7iBpEHGVNc&t=1038s
    constexpr size_t n = 3;
    float scores[n] = {2.2, 2.5, -3.1};
    size_t m = 2;
    float actual = lfuns_svm(n, &scores[0], m);
    float expected = 12.9;
    float eps = 0.0001;
    float lower = expected - eps;
    float upper = expected + eps;
    cr_assert(lower < actual && actual < upper);
}
