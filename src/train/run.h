#ifndef RUN_H
#define RUN_H
#include <stddef.h>


struct inputs {
    const size_t nepochs;
    const float learning_rate;
    const bool standardize;
    const bool verbose;
    const char * features_path;
    const char * labels_path;
};

void run (struct inputs inputs);

#endif
