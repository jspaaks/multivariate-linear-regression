#ifndef KWARGS_H
#define KWARGS_H
#include <stddef.h>
#include <stdio.h>


typedef struct kwargs_class KwargsClass;


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


void kwargs_classify (int argc, const char * argv[],
                      size_t nclasses, const KwargsClass * classes,
                      size_t * nclassifieds, KwargsType * classifieds);


KwargsType * kwargs_create (int argc);


void kwargs_destroy (KwargsType ** classifieds);


const char * kwargs_get_optional_value (const char * name,
                                        int argc, const char * argv[],
                                        size_t nclasses, const KwargsClass * classes,
                                        size_t nclassifieds, const KwargsType * classifieds);


const char * kwargs_get_positional_value (size_t ipos,
                                          int argc, const char * argv[],
                                          size_t nclassifieds, const KwargsType * classifieds);


const char * kwargs_get_required_value (const char * name,
                                        int argc, const char * argv[],
                                        size_t nclasses, const KwargsClass * classes,
                                        size_t nclassifieds, const KwargsType * classifieds);


int kwargs_has_flag (const char * name,
                     int argc, const char * argv[],
                     size_t nclasses, const KwargsClass * classes,
                     size_t nclassifieds, const KwargsType * classifieds);


int kwargs_has_optional (const char * name,
                         int argc, const char * argv[],
                         size_t nclasses, const KwargsClass * classes,
                         size_t nclassifieds, const KwargsType * classifieds);


void kwargs_print_classifications (FILE * stream,
                                   int argc, const char * argv[],
                                   size_t nclassifieds, const KwargsType * classifieds);

#endif
