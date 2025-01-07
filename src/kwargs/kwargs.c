#include "kwargs/kwargs.h"
#include "protected.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void kwargs_classify (Kwargs * kwargs) {

    // assert no duplicate names
    // assert not both longname and shortname nullptrs
    // assert spelling and length of shortname
    // assert spelling and length of longname

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


Kwargs * kwargs_create (int argc, const char * argv[], size_t nclasses, const KwargsClass * classes) {
    errno = 0;
    Kwargs * kwargs = calloc(1, sizeof(Kwargs));
    if (kwargs == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for instance of \"Kwargs\", aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }

    errno = 0;
    KwargsType * classifieds = calloc(argc, sizeof(KwargsType));
    if (classifieds == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for array of \"KwargsType\", aborting.\n", strerror(errno));
        errno = 0;
        kwargs_destroy(&kwargs);
        exit(EXIT_FAILURE);
    }
    kwargs->argc = argc;
    kwargs->argv = argv;
    kwargs->nclasses = nclasses;
    kwargs->classes = classes;
    kwargs->nclassifieds = (size_t) argc;
    kwargs->classifieds = classifieds;
    return kwargs;
}


void kwargs_destroy (Kwargs ** kwargs) {
    free((*kwargs)->classifieds);
    (*kwargs)->classifieds = nullptr;
    free(*kwargs);
    *kwargs = nullptr;
}


const char * kwargs_get_optional_value (const char * name, Kwargs * kwargs) {
    int iarg = has_type(name, kwargs, KWARGS_OPTIONAL);
    if (iarg == 0) {
        return nullptr;
    }
    if (iarg + 1 >= kwargs->argc) {
        fprintf(stderr, "Value for \"%s\" seems to be missing, aborting.\n", name);
        kwargs_destroy(&kwargs);
        exit(EXIT_FAILURE);
    }
    return kwargs->argv[iarg + 1];
}


const char * kwargs_get_positional_value (size_t ipos, Kwargs * kwargs) {
    size_t k = 0;
    for (size_t i = 0; i < kwargs->nclassifieds; i++) {
        if (kwargs->classifieds[i] == KWARGS_POSITIONAL) {
            if (k == ipos) {
                return kwargs->argv[i];
            } else {
                k++;
            }
        }
    }
    fprintf(stderr, "ERROR: requested positional argument %zu doesn't exist, aborting.\n", ipos);
    kwargs_destroy(&kwargs);
    exit(EXIT_FAILURE);
}


const char * kwargs_get_required_value (const char * name, Kwargs * kwargs) {
    int iarg = has_type(name, kwargs, KWARGS_REQUIRED);
    if (iarg == 0) {
        return nullptr;
    }
    if (iarg + 1 >= kwargs->argc) {
        fprintf(stderr, "Value for \"%s\" seems to be missing, aborting.\n", name);
        kwargs_destroy(&kwargs);
        exit(EXIT_FAILURE);
    }
    return kwargs->argv[iarg + 1];
}


int kwargs_has_flag (const char * name, Kwargs * kwargs) {
    return has_type(name, kwargs, KWARGS_FLAG);
}


int kwargs_has_optional (const char * name, Kwargs * kwargs) {
    return has_type(name, kwargs, KWARGS_OPTIONAL);
}


void kwargs_print_classifications (FILE * stream, Kwargs * kwargs) {
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
    for (size_t i = 0; i < kwargs->nclassifieds; i++) {
        int itype = kwargs->classifieds[i];
        fprintf(stream, "%-*s %s\n", ell, kwargs->argv[i], typestrings[itype]);
    }
}
