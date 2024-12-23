#include "data.h"
#include "matrix/matrix.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void populate_features (const char * path, Matrix * features) {
    errno = 0;
    FILE * fp = fopen(path, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    constexpr size_t bufsize = 100;
    char buffer[bufsize] = {};
    for (size_t ir = 0; ir < features->nr; ir++) {
        float * intercept = &features->vals[ir * features->nc + 0];
        float * area      = &features->vals[ir * features->nc + 1];
        float * bedrooms  = &features->vals[ir * features->nc + 2];
        *intercept = 1.0f;
        fgets(buffer, bufsize, fp);
        sscanf(buffer, "%f,%f,%*f\n", area, bedrooms);
    }
    fclose(fp);
}


void populate_labels (const char * path, Matrix * labels) {
    errno = 0;
    FILE * fp = fopen(path, "r");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    constexpr size_t bufsize = 100;
    char buffer[bufsize] = {};
    for (size_t ir = 0; ir < labels->nr; ir++) {
        float * price = &labels->vals[ir];
        fgets(buffer, bufsize, fp);
        sscanf(buffer, "%*f,%*f,%f\n", price);
    }
    fclose(fp);
}
