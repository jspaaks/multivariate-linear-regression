#ifndef KWARGS_H
#define KWARGS_H
#include <stddef.h>
#include <stdio.h>


typedef struct kwargs_class KwargsClass;
typedef struct kwargs Kwargs;


typedef enum {
    KWARGS_UNKNOWN = 0,
    KWARGS_EXE,
    KWARGS_FLAG,
    KWARGS_OPTIONAL,
    KWARGS_POSITIONAL,
    KWARGS_REQUIRED,
    KWARGS_VALUE
} KwargsType;


struct kwargs_class {
    char * longname;
    char * shortname;
    KwargsType type;
};

struct kwargs {
    int argc;
    const char ** argv;
    size_t nclasses;
    const KwargsClass * classes;
    size_t nclassifieds;
    KwargsType * classifieds;
};


void kwargs_classify (Kwargs * kwargs);
Kwargs * kwargs_create (int argc, const char * argv[], size_t nclasses, const KwargsClass * classes);
void kwargs_destroy (Kwargs ** kwargs);
const char * kwargs_get_optional_value (const char * name, Kwargs * kwargs);
const char * kwargs_get_positional_value (size_t ipos, Kwargs * kwargs);
const char * kwargs_get_required_value (const char * name, Kwargs * kwargs);
int kwargs_has_flag (const char * name, Kwargs * kwargs);
int kwargs_has_optional (const char * name, Kwargs * kwargs);
void kwargs_print_classifications (FILE * stream, Kwargs * kwargs);

#endif
