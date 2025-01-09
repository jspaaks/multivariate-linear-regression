#include "data.h"
#include "matrix/matrix.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void populate_features (const char * path, Matrix * features_raw) {
    errno = 0;
    FILE * fp = fopen(path, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    constexpr size_t bufsize = 100;
    char buffer[bufsize] = {};
    size_t nr = features_raw->nr;
    size_t nc = features_raw->nc;
    for (size_t ir = 0; ir < nr; ir++) {
        float * intercept = &features_raw->xs[ir * nc + 0];
        float * area      = &features_raw->xs[ir * nc + 1];
        float * bedrooms  = &features_raw->xs[ir * nc + 2];
        *intercept = 1.0f;
        fgets(buffer, bufsize, fp);
        sscanf(buffer, "%f,%f,%*f\n", area, bedrooms);
    }
    fclose(fp);
}


void populate_labels (const char * path, Matrix * labels_raw_transp) {
    errno = 0;
    FILE * fp = fopen(path, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    constexpr size_t bufsize = 100;
    char buffer[bufsize] = {};
    for (size_t ic = 0; ic < labels_raw_transp->nc; ic++) {
        float * price = &labels_raw_transp->xs[ic];
        fgets(buffer, bufsize, fp);
        sscanf(buffer, "%*f,%*f,%f\n", price);
    }
    fclose(fp);
}
