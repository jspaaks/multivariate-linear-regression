#ifndef AFUNS_H
#define AFUNS_H

float afuns_leaky_relu (float);
float afuns_leaky_relu_deriv (float);
float afuns_parametric_relu (float, float);
float afuns_parametric_relu_deriv (float, float);
float afuns_passthrough (float);
float afuns_passthrough_deriv (void);
float afuns_relu (float);
float afuns_relu_deriv (float);
float afuns_sigmoid (float);
float afuns_sigmoid_deriv (float);

#endif
