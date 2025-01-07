#ifndef KWARGS_PROTECTED_H
#define KWARGS_PROTECTED_H
#include "kwargs/kwargs.h"
#include <stddef.h>

const KwargsClass * get_class (const char * name, Kwargs * kwargs);
int has_type (const char * name, Kwargs * kwargs, KwargsType type);

#endif
