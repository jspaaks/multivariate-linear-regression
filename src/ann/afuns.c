#include "ann/afuns.h"
#include "math.h"

// various activation functions

float ann__afuns_leaky_relu (float x) {
    return ann__afuns_parametric_relu(0.01f, x);
}


float ann__afuns_leaky_relu_deriv (float x) {
    return ann__afuns_parametric_relu_deriv(0.01f, x);
}


float ann__afuns_parametric_relu (float a, float x) {
    if (x > 0.0f) {
        return x;
    }
    return a * x;
}


float ann__afuns_parametric_relu_deriv (float a, float x) {
    if (x > 0.0f) {
        return 1.0f;
    }
    return a;
}


float ann__afuns_relu (float x) {
    return ann__afuns_parametric_relu(0.0f, x);
}


float ann__afuns_relu_deriv (float x) {
    return ann__afuns_parametric_relu_deriv(0.0f, x);
}


float ann__afuns_sigmoid (float x) {
    return 1.0f / (1.0f + expf(-x));
}


float ann__afuns_sigmoid_deriv (float x) {
    float z = ann__afuns_sigmoid (x);
    return z * (1 - z);
}
