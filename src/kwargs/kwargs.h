#ifndef KWARGS_H
#define KWARGS_H
#include <stddef.h>

typedef struct recognized_key RecognizedKey;


struct recognized_key {
    bool isrequired;
    char * longname;
    char * shortname;
    bool takesvalue;
};


struct marked {
    const struct recognized_key * parent;
    enum {
        KWARGS_OTHER = 0,
        KWARGS_EXENAME,
        KWARGS_OPTIONAL,
        KWARGS_REQUIRED,
        KWARGS_VALUE,
        KWARGS_POSITIONAL
    } argtype;
};

#endif
