#include "randu.h"
#include <stdlib.h>

/*
 * Draw a random number from a uniform unit distribution [0,1]
 */
double randu (void) {
    int z = rand();
    double zd = (double) z;
    return zd / RAND_MAX;
}
