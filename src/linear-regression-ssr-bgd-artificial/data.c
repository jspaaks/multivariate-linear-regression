#include "data.h"
#include "boxmuller/boxmuller.h"
#include "matrix/matrix.h"
#include <assert.h>
#include <stdlib.h>


void populate_features (Matrix * features) {
    for (size_t ir = 0; ir < features->nr; ir++) {
        features->vals[ir * features->nc] = 1;
        for (size_t ic = 1; ic < features->nc; ic++) {
            size_t i = ir * features->nc + ic;
            features->vals[i] = ((float) (rand() % 200 - 100)) / 100;
        }
    }
}


void populate_labels (const Matrix * true_weights, const Matrix * features_transp, Matrix * labels_transp, Matrix * true_residuals_transp) {
    float sigma = 10.0f;
    for (size_t i = 0; i < true_residuals_transp->n; i++) {
        true_residuals_transp->vals[i] = (float) (sigma * boxmuller_randn());
    }
    matrix_dotpro(true_weights, features_transp, labels_transp);
    matrix_ebeadd(labels_transp, true_residuals_transp, labels_transp);
}


void populate_true_weights (Matrix * true_weights) {
    assert(true_weights->nr == 1);
    assert(true_weights->nc == 3);
    assert(true_weights->n == 3);
    true_weights->vals[0] = 9.87;
    true_weights->vals[1] = 6.54;
    true_weights->vals[2] = 3.21;
}
