#include "lfuns/lfuns.h"
#include <math.h>
#include <stdlib.h>


float lfuns_softmax(const size_t n, const float * values, const size_t m) {
    /*  softmax also known as
     *    - multinomial logistic regression
     *    - cross-entropy loss
     */
    float summed = 0.0;
    float * tmp = calloc(n, sizeof(float));
    // exponentiate each value in values
    for (size_t i = 0; i < n; i++) {
        float x = expf(values[i]);
        tmp[i] = x;
        summed += x;
    }
    // Examples from https://www.youtube.com/watch?v=h7iBpEHGVNc&t=2698s use
    // log10, not ln. Not sure it matters since both are monotonic, but I
    // expected `ln`.
    float rv = -1 * log10f(tmp[m] / summed);
    free(tmp);
    return rv;
}


float lfuns_svm(const size_t n, const float * values, const size_t m) {
    float summed = 0.0;
    for (size_t i = 0; i < n; i++) {
        float loss = values[i] - values[m] + 1;
        summed += loss > 0 ? loss : 0;
    }
    return summed - 1.0;
}
