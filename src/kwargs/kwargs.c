#include "kwargs/kwargs.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const KwargsValid * get_parent (char * actual_arg, size_t nvalids, const KwargsValid * valids);
bool name_matches(KwargsParsed * parsed, char * name);
bool requires_value (KwargsParsed * parsed);


const KwargsValid * get_parent (char * actual_arg, size_t nvalids, const KwargsValid * valids) {
    for (size_t i = 0; i < nvalids; i++) {
        char * longname = valids[i].longname;
        char * shortname = valids[i].shortname;
        bool longname_matches = longname != nullptr && strncmp(actual_arg, longname, strlen(longname)) == 0;
        bool shortname_matches = shortname != nullptr && strncmp(actual_arg, shortname, strlen(shortname)) == 0;
        if (longname_matches || shortname_matches) return &valids[i];
    }
    return nullptr;
}


KwargsParsed * kwargs_create (int argc) {
    errno = 0;
    KwargsParsed * parseds = calloc(argc, sizeof(KwargsParsed));
    if (parseds == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for \"KwargsParsed\" instance, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }
    return parseds;
}


void kwargs_destroy (KwargsParsed ** parseds) {
    free(*parseds);
    *parseds = nullptr;
}


bool kwargs_has_flag (size_t nparseds, KwargsParsed * parseds, char * name) {
    for (size_t i = 0; i < nparseds; i++) {
        if (parseds[i].type != KWARGS_FLAG) continue;
        if (name_matches(&parseds[i], name)) return true;
    }
    return false;
}


bool kwargs_has_optional (size_t nparseds, KwargsParsed * parseds, char * name) {
    for (size_t i = 0; i < nparseds; i++) {
        if (parseds[i].type != KWARGS_OPTIONAL) continue;
        if (name_matches(&parseds[i], name)) return true;
    }
    return false;
}


char * kwargs_get_optional_value (size_t nparseds, KwargsParsed * parseds, char * name) {
    bool found = false;
    for (size_t i = 0; i < nparseds; i++) {
        size_t j = i + 1;
        bool a = parseds[i].type == KWARGS_OPTIONAL;
        bool b = name_matches(&parseds[i], name);
        bool c = j < nparseds;
        bool d = parseds[j].type == KWARGS_VALUE;
        if (a && b) found = true;
        if ((found && !c) || (found && !d)) {
            fprintf(stderr, "ERROR: requested key \"%s\" found but seems to be missing its value, aborting.\n", name);
            exit(EXIT_FAILURE);
        }
        if (found && c && d) return parseds[j].value;
    }
    fprintf(stderr, "ERROR: requested optional argument \"%s\" doesn't exist, aborting.\n", name);
    exit(EXIT_FAILURE);
}


char * kwargs_get_positional_value (size_t nparseds, KwargsParsed * parseds, size_t ipos) {
    size_t k = 0;
    for (size_t i = 0; i < nparseds; i++) {
        if (parseds[i].type == KWARGS_POSITIONAL) {
            if (k == ipos) {
                return parseds[i].value;
            } else {
                k++;
            }
        }
    }
    fprintf(stderr, "ERROR: requested positional argument %zu doesn't exist, aborting.\n", ipos);
    exit(EXIT_FAILURE);
}


char * kwargs_get_required_value (size_t nparseds, KwargsParsed * parseds, char * name) {
    bool found = false;
    for (size_t i = 0; i < nparseds; i++) {
        size_t j = i + 1;
        bool a = parseds[i].type == KWARGS_REQUIRED;
        bool b = name_matches(&parseds[i], name);
        bool c = j < nparseds;
        bool d = parseds[j].type == KWARGS_VALUE;
        if (a && b) found = true;
        if ((found && !c) || (found && !d)) {
            fprintf(stderr, "ERROR: requested key \"%s\" found but seems to be missing its value, aborting.\n", name);
            exit(EXIT_FAILURE);
        }
        if (found && c && d) return parseds[j].value;
    }
    fprintf(stderr, "ERROR: requested required argument \"%s\" doesn't exist, aborting.\n", name);
    exit(EXIT_FAILURE);
}


void kwargs_parse (char * argv[], size_t nvalids, const KwargsValid * valids, size_t nparseds, KwargsParsed * parseds) {
    // assert no duplicate names
    // assert not both longname and shortname nullptrs
    // assert spelling and length of shortname
    // assert spelling and length of longname
    // assert required args are present

    parseds[0] = (KwargsParsed){
        .longname = nullptr,
        .parent = nullptr,
        .shortname = nullptr,
        .type = KWARGS_EXENAME,
        .value = argv[0]
    };

    for (size_t i = 1; i < nparseds; i++) {
        char * arg = argv[i];
        // if previous arg was positional, so is this one
        if (parseds[i - 1].type == KWARGS_POSITIONAL) {
            parseds[i] = (KwargsParsed) {
                .longname = nullptr,
                .parent = nullptr,
                .shortname = nullptr,
                .type = KWARGS_POSITIONAL,
                .value = arg
            };
            continue;
        }
        // if previous arg required a value, mark current as KWARGS_VALUE
        if (requires_value(&parseds[i - 1])) {
            parseds[i] = (KwargsParsed) {
                .longname = nullptr,
                .parent = nullptr,
                .shortname = nullptr,
                .type = KWARGS_VALUE,
                .value = arg
            };
            continue;
        }
        const KwargsValid * parent = get_parent(arg, nvalids, valids);
        if (parent == nullptr) {
            parseds[i] = (KwargsParsed) {
                .longname = nullptr,
                .parent = parent,
                .shortname = nullptr,
                .type = KWARGS_POSITIONAL,
                .value = arg
            };
        } else {
            parseds[i] = (KwargsParsed) {
                .longname = parent->longname,
                .parent = parent,
                .shortname = parent->shortname,
                .type = parent->type,
                .value = parent->type == KWARGS_VALUE ? nullptr : arg
            };
        }
    }
}


void kwargs_print_results (FILE * stream, size_t nparseds, KwargsParsed * parseds) {
    int ell = 25;
    const char typestrings[][20] = {
        "other",
        "exe",
        "flag",
        "optional",
        "positional",
        "required",
        "value"
    };
    for (size_t i = 0; i < nparseds; i++) {
        char * value = parseds[i].value;
        int itype = parseds[i].type;
        fprintf(stream, "%-*s %s\n", ell, value, typestrings[itype]);
    }
}


bool name_matches(KwargsParsed * parsed, char * name) {
        bool a = parsed->longname != nullptr &&
                 strncmp(parsed->longname, name, strlen(parsed->longname) + 1) == 0;
        bool b = parsed->shortname != nullptr &&
                 strncmp(parsed->shortname, name, strlen(parsed->shortname) + 1) == 0;
        return a || b;
}


bool requires_value (KwargsParsed * parsed) {
    if (parsed->type == KWARGS_OPTIONAL) return true;
    if (parsed->type == KWARGS_REQUIRED) return true;
    return false;
}
