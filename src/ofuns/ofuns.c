#include "ofuns/ofuns.h"


float ofuns_svm(size_t n, float * values, size_t m) {
    float s0 = 0.0;
    for (size_t i = 0; i < m; i++) {
        s0 += values[i];
    }
    for (size_t i = m + 1; i < n; i++) {
        s0 += values[i];
    }
    float s1 = values[m];

    float loss = s1 - s0 + 1;
    return loss > 0 ? loss : 0;
}
