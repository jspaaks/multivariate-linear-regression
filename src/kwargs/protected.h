#ifndef KWARGS_PROTECTED_H
#define KWARGS_PROTECTED_H
#include "kwargs/kwargs.h"
#include <stddef.h>

const KwargsClass * get_class (const char * name, size_t nclasses, const KwargsClass * classes);
int has_type (const char * name, int argc, const char * argv[], size_t nclasses, const KwargsClass * classes, size_t nclassifieds, const KwargsType * classifieds, KwargsType type);

#endif
