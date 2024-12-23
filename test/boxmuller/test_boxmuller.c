#include "boxmuller/boxmuller.h"
#include <assert.h>
#include <criterion/criterion.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int compdbl(const void * a, const void * b);
double kolmogorov_smirnov(size_t n, double * array);
double standard_normal_cdf(double x);


double kolmogorov_smirnov(size_t nsamples, double * array) {
    const size_t nbins = 30;
    const double lower = -3.0;
    const double upper = 3.0;
    const double w = (upper - lower) / nbins; // [-3.0, +3.0] contains 99.7% of the density mass
    qsort(array, nsamples, sizeof(double), compdbl);
    double maxdiff = 0.0;
    size_t count = 0;
    double * p = &array[0];
    for (size_t i = 1; i <= nbins; i++) {
        double binupper = lower + i * w;
        double fx = standard_normal_cdf(binupper);
        while (*p < binupper && count < nsamples) {
            count++;
            p++;
        }
        double sx = (double) count / nsamples;
        double diff = fabs(sx - fx);
        if (diff > maxdiff) {
            maxdiff = diff;
        }
    }
    return maxdiff;
}


double standard_normal_cdf(double x) {
    return 0.5 * (1 + erf(x / sqrt(2)));
}

int compdbl(const void * a, const void * b) {
    const double * aa = (const double *) a;
    const double * bb = (const double *) b;

    if (*aa < *bb) return -1;
    if (*aa > *bb) return 1;
    return 0;
}


Test(boxmuller, randn) {
    constexpr size_t n = 40;
    const double ks_thres = 0.210;  // critical value for n = 40, alpha = 0.05
    srand(time(nullptr));
    double numbers[n] = {};
    for (size_t i = 0; i < n; i++) {
        numbers[i] = boxmuller_randn();
    }
    double ks_actual = kolmogorov_smirnov(n, numbers);
    cr_assert(ks_actual < ks_thres && "ks_actual unexpectedly exceeded ks_thres");
}
