#include "rfuns/rfuns.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

// https://www.youtube.com/watch?v=h7iBpEHGVNc&t=1868s

float rfuns_ell_1 (size_t n, float * weights) {
    float summed = 0.0;
    for (size_t i = 0; i < n; i++) {
        summed += fabsf(weights[i]);
    }
    return summed;
}


float rfuns_ell_2 (size_t n, float * weights) {
    float summed = 0.0;
    for (size_t i = 0; i < n; i++) {
        summed += pow(weights[i], 2);
    }
    return summed;
}
