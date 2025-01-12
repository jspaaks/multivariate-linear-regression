#ifndef PLOTTING_H
#define PLOTTING_H
#include "matrix/matrix.h"
#include <plplot/plplot.h>

void plot_losses (PLCHAR_VECTOR device, size_t nepochs, const Matrix * epochs, const Matrix * losses, size_t nsamples);

#endif
