#include "options.h"
#include "kwargs/kwargs.h"
#include <string.h>
#include <stdlib.h>


float get_learning_rate (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value ("--learning_rate", kwargs);
    if (s == nullptr) {
        return (float) 0.01;
    }
    float learning_rate;
    sscanf(s, " %f", &learning_rate);
    return learning_rate;
}


size_t get_nepochs (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value ("--nepochs", kwargs);
    if (s == nullptr) {
        return 10;
    }
    size_t nepochs;
    sscanf(s, " %zu", &nepochs);
    return nepochs;
}
