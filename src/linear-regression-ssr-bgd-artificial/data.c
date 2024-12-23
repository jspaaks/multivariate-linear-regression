#include "data.h"
#include "matrix/matrix.h"
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


void populate_labels (const Matrix * true_weights, const Matrix * features_transp, Matrix * labels_transp) {
    matrix_dotpro(true_weights, features_transp, labels_transp);
}


