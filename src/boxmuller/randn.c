#include <boxmuller/boxmuller.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

double randu(void);


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


/* 
 * Draw a random number from a uniform unit distribution [0,1]
 */
double randu (void) {
    int z = rand();
    double zd = (double) z;
    return zd / RAND_MAX;
}
