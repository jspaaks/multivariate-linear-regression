#include "plotting.h"
#include <plplot/plplot.h>
#include <float.h>
#include <stdlib.h>


typedef struct limits {
    PLFLT xmin;
    PLFLT xmax;
    PLFLT ymin;
    PLFLT ymax;
} Limits;


PLFLT_VECTOR create_x (size_t n, const Matrix * epochs);
PLFLT_VECTOR create_y (size_t n, const Matrix * losses);
void destroy_x (PLFLT_VECTOR * x);
void destroy_y (PLFLT_VECTOR * y);


PLFLT_VECTOR create_x (const size_t n, const Matrix * epochs) {
    PLFLT * x = calloc(n, sizeof(PLFLT));
    for (size_t i = 0; i < n; i++) {
        x[i] = (PLFLT) epochs->xs[i];
    }
    return (PLFLT_VECTOR) x;
}


PLFLT_VECTOR create_y (const size_t n, const Matrix * losses) {
    PLFLT * y = calloc(n, sizeof(PLFLT));
    for (size_t i = 0; i < n; i++) {
        y[i] = (PLFLT) losses->xs[i];
    }
    return (PLFLT_VECTOR) y;
}

void plot_losses (PLCHAR_VECTOR device, const size_t nepochs, const Matrix * epochs, const Matrix * losses, const size_t nsamples) {

    PLFLT_VECTOR x = create_x(1 + nepochs, epochs);
    PLFLT_VECTOR y = create_y(1 + nepochs, losses);

    // choose the device
    plsdev(device);

    // specify the entries of the colormap
    {
        size_t i = 0;
        plscol0(i++,   0,   0,   0); // black
        plscol0(i++,  32,  32,  32); // 12.5% gray
        plscol0(i++,  64,  64,  64); // 25% gray
        plscol0(i++, 128, 128, 128); // 50% gray
        plscol0(i++, 192, 192, 192); // 75% gray
        plscol0(i++, 255, 255, 255); // white
        plscol0(i++,   0, 128, 255); // cyan
        plscol0(i++, 255,   0, 128); // magenta
        plscol0(i++, 255, 128,  50); // orange
        plscol0(i++,  32, 128,  32); // forest green
        plscol0(i++,  64,   0,  64); // deep purple
    }

    // initialize plplot
    plinit();

    // pick a color for drawing the axes and such
    plcol0(4);

    // whether to send interactive windows into wait mode on 'plenv'
    plspause(true);

    Limits ulimits = {
        .xmin = 0.15,
        .xmax = 0.95,
        .ymin = 0.15,
        .ymax = 0.9
    };

    float ymax = FLT_MIN;
    for (size_t i = 0; i < losses->n; i++) {
        float yi = losses->xs[i];
        ymax = yi > ymax ? yi : ymax;
    }

    Limits wlimits = {
        .xmin = 0.0,
        .xmax = nepochs,
        .ymin = 0.0,
        .ymax = ymax
    };

    // advance the page ...?
    pladv(0);

    // unit coordinates of the edges of the viewport
    plvpor(ulimits.xmin, ulimits.xmax, ulimits.ymin, ulimits.ymax);

    // world coordinates of the edges of the viewport
    plwind(wlimits.xmin, wlimits.xmax, wlimits.ymin, wlimits.ymax);

    // draw the viewport box, axes, and all of their options
    PLCHAR_VECTOR xopt = "abcgnst";
    PLFLT xtick = 0.0;
    PLINT nxsub = 0;
    PLCHAR_VECTOR yopt = "abcgnst";
    PLFLT ytick = 0.0;
    PLINT nysub = 0;
    plbox(xopt, xtick, nxsub, yopt, ytick, nysub);

    // xlabel, ylabel, title
    {
        char title[100];
        sprintf(title, "n#dsamples#u = %zu", nsamples);
        pllab("epoch", "#[0x00BD] SSR", title);
    }

    // pick a plotting color
    plcol0(7);

    // set line width
    plwidth(2.0);

    // plot the epoch v losses data
    plline(nepochs + 1, x, y);

    // clean up memory resources and end the session
    plend();

    // unconst the pointers, then clean up memory resources
    free((void *) x);
    free((void *) y);
}
