#include "lfuns/lfuns.h"


float lfuns_svm(size_t n, float * values, size_t m) {
    float summed = 0.0;
    for (size_t i = 0; i < n; i++) {
        float loss = values[i] - values[m] + 1;
        summed += loss > 0 ? loss : 0;
    }
    return summed - 1.0;
}
