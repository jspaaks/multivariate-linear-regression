#include "protected.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>


const KwargsClass * get_class (const char * name, const Kwargs * kwargs) {
    for (size_t i = 0; i < kwargs->nclasses; i++) {
        {
            char * longname = kwargs->classes[i].longname;
            bool matches = longname != nullptr &&
                           strncmp(longname, name, strlen(longname) + 1) == 0;
            if (matches) return &kwargs->classes[i];
        }
        {
            char * shortname = kwargs->classes[i].shortname;
            bool matches = shortname != nullptr &&
                           strncmp(shortname, name, strlen(shortname) + 1) == 0;
            if (matches) return &kwargs->classes[i];
        }
    }
    return nullptr;
}


void classify (Kwargs * kwargs) {

    // assert no duplicate names
    // assert not both longname and shortname nullptrs
    // assert spelling and length of shortname
    // assert spelling and length of longname
    // assert user supplied types are in [flag, optional, required]

    kwargs->classifieds[0] = KWARGS_EXE;

    for (size_t icurr = 1; icurr < kwargs->nclassifieds; icurr++) {
        size_t iprev = icurr - 1;
        // if previous arg was positional, so is this one
        if (kwargs->classifieds[iprev] == KWARGS_POSITIONAL) {
            kwargs->classifieds[icurr] = KWARGS_POSITIONAL;
            continue;
        }
        // if previous arg required a value, mark current as KWARGS_VALUE
        if ((kwargs->classifieds[iprev] == KWARGS_OPTIONAL) || (kwargs->classifieds[iprev] == KWARGS_REQUIRED)) {
            kwargs->classifieds[icurr] = KWARGS_VALUE;
            continue;
        }
        const KwargsClass * cls = get_class(kwargs->argv[icurr], kwargs);
        if (cls == nullptr) {
            kwargs->classifieds[icurr] = KWARGS_POSITIONAL;
            continue;
        } else {
            kwargs->classifieds[icurr] = cls->type;
        }
    }
    // assert required args are present
    for (size_t i = 0; i < kwargs->nclasses; i++) {
        if (kwargs->classes[i].type != KWARGS_REQUIRED) continue;
        int iarg = has_type(kwargs->classes[i].longname, kwargs, KWARGS_REQUIRED);
        if (iarg == 0) {
            fprintf(stderr, "ERROR: Required parameter \"%s\" seems to be missing, aborting.\n", kwargs->classes[i].longname);
            kwargs_destroy(&kwargs);
            exit(EXIT_FAILURE);
        }
    }
}


int has_type (const char * name, const Kwargs * kwargs, KwargsType type) {
    const char typenames[][20] = {
        "other",
        "exe",
        "flag",
        "optional",
        "positional",
        "required",
        "value"
    };
    const KwargsClass * cls = get_class(name, kwargs);
    // test whether name is known
    if (cls == nullptr) {
        fprintf(stderr, "ERROR: %s name \"%s\" doesn't exist, aborting.\n", typenames[type], name);
        exit(EXIT_FAILURE);
    }
    // test whether name is of type 'type'
    if (cls->type != type) {
        fprintf(stderr, "ERROR: name \"%s\" exists but is not a %s, aborting.\n", name, typenames[type]);
        exit(EXIT_FAILURE);
    }
    // test where name is used in argv
    char * names[2] = {cls->shortname, cls->longname};
    for (size_t iname = 0; iname < 2; iname++) {
        size_t ell = strlen(names[iname]) + 1;
        for (int iarg = 1; iarg < kwargs->argc; iarg++) {
            bool cond = strncmp(names[iname], kwargs->argv[iarg], ell) == 0;
            if (cond) {
                // test whether classification is as expected
                if (cls->type != kwargs->classifieds[iarg]) {
                    KwargsType itype = kwargs->classifieds[iarg];
                    fprintf(stdout, "ERROR: Name \"%s\" seems to have been misclassified as a \"%s\"\n"
                                    "argument. Check the spelling of preceding parameter names, whether\n"
                                    "preceding parameter names that require a value did in fact get one,\n"
                                    "and verify that all preceding parameter names are valid shortnames\n"
                                    "or longnames.\n", kwargs->argv[iarg], typenames[itype]);
                    exit(EXIT_FAILURE);
                }
                return iarg;
            }
        }
    }
    return 0;
}
