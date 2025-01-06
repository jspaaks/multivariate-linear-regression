#ifndef KWARGS_H
#define KWARGS_H
#include <stddef.h>
#include <stdio.h>

typedef struct kwargs_valid KwargsValid;
typedef struct kwargs_parsed KwargsParsed;
typedef struct kwargs_results KwargsResults;

typedef enum {
    KWARGS_OTHER = 0,
    KWARGS_EXENAME,
    KWARGS_FLAG,
    KWARGS_OPTIONAL,
    KWARGS_POSITIONAL,
    KWARGS_REQUIRED,
    KWARGS_VALUE
} KwargsType;

struct kwargs_valid {
    char * longname;
    char * shortname;
    KwargsType type;
};

struct kwargs_parsed {
    char * longname;
    const struct kwargs_valid * parent;
    char * shortname;
    KwargsType type;
    char * value;
};

KwargsParsed * kwargs_create (int argc);
void kwargs_destroy (KwargsParsed ** parseds);
char * kwargs_get_optional_value (size_t nparseds, KwargsParsed * parseds, char * name);
char * kwargs_get_positional_value (size_t nparseds, KwargsParsed * parseds, size_t ipos);
char * kwargs_get_required_value (size_t nparseds, KwargsParsed * parseds, char * name);
bool kwargs_has_flag (size_t nparseds, KwargsParsed * parseds, char * name);
bool kwargs_has_optional (size_t nparseds, KwargsParsed * parseds, char * name);
void kwargs_parse (char * argv[], size_t nvalids, const KwargsValid * valids, size_t nparseds, KwargsParsed * parseds);
void kwargs_print_results (FILE * stream, size_t nparseds, KwargsParsed * parseds);

#endif
