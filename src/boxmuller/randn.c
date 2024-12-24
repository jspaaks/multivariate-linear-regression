#include <boxmuller/boxmuller.h>
#include <math.h>
#include <stdlib.h>
#include "randu.h"


/*
 *  Draw a random number from the standard normal distribution
 *  https://en.wikipedia.org/wiki/Box–Muller_transform
 */
double boxmuller_randn (void) {
    double u;
    double r2;
    do {
        double v;
        u = randu() * 2.0 - 1.0;
        v = randu() * 2.0 - 1.0;
        r2 = u * u + v * v;
    } while (r2 > 1.0 || r2 == 0.0);
    return u * sqrt(-2.0 * log(r2) / r2);
}
