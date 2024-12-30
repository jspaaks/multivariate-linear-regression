#ifndef DATA_H
#define DATA_H
#include "matrix/matrix.h"

void populate_features (Matrix * features);
void populate_labels (const Matrix * true_weights, const Matrix * features_transp, Matrix * labels_transp, Matrix * true_residuals_transp);
void populate_true_weights (Matrix * true_weights);

#endif
