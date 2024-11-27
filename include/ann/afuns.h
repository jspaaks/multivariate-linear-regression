#ifndef ANN__AFUNS_H
#define ANN__AFUNS_H

float leaky_relu (float);
float leaky_relu_deriv (float)
float parametric_relu (float, float);
float parametric_relu_deriv (float, float);
float relu (float);
float relu_deriv (float);
float sigmoid (float);
float sigmoid_deriv (float);

#endif
