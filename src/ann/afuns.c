#include "ann/afuns.h"
#include "math.h"

// various activation functions

float leaky_relu (float x) {
    return parametric_relu(0.01f, x);
}


float leaky_relu_deriv (float x) {
    return parametric_relu_deriv(0.01f, x);
}


float parametric_relu (float a, float x) {
    if (x > 0.0f) {
        return x;
    }
    return a * x;
}


float parametric_relu_deriv (float a, float x) {
    if (x > 0.0f) {
        return 1.0f;
    }
    return a;
}


float relu (float x) {
    return parametric_relu(0.0f, x);
}


float relu_deriv (float x) {
    return parametric_relu_deriv(0.0f, x);
}


float sigmoid (float x) {
    return 1.0f / (1.0f + expf(-x));
}


float sigmoid_deriv (float x) {
    float z = sigmoid (x);
    return z * (1 - z);
}
