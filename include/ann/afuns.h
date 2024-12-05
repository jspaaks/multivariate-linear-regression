#ifndef ANN__AFUNS_H
#define ANN__AFUNS_H

float ann__afuns_leaky_relu (float);
float ann__afuns_leaky_relu_deriv (float);
float ann__afuns_parametric_relu (float, float);
float ann__afuns_parametric_relu_deriv (float, float);
float ann__afuns_relu (float);
float ann__afuns_relu_deriv (float);
float ann__afuns_sigmoid (float);
float ann__afuns_sigmoid_deriv (float);

#endif
