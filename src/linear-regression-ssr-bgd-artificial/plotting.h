#ifndef PLOTTING_H
#define PLOTTING_H
#include "matrix/matrix.h"
#include <plplot/plplot.h>

void plot_residuals (PLCHAR_VECTOR device, const Matrix * iterations, const Matrix * avg_residuals, size_t niters, size_t ni);

#endif
