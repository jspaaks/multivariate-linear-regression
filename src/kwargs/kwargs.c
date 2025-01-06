#include "kwargs/kwargs.h"
#include "protected.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void kwargs_classify (int argc, const char * argv[],
                      size_t nclasses, const KwargsClass * classes,
                      size_t * nclassifieds, KwargsType * classifieds) {

    *nclassifieds = (size_t) argc;

    // assert no duplicate names
    // assert not both longname and shortname nullptrs
    // assert spelling and length of shortname
    // assert spelling and length of longname

    classifieds[0] = KWARGS_EXE;

    for (size_t icurr = 1; icurr < *nclassifieds; icurr++) {
        size_t iprev = icurr - 1;
        // if previous arg was positional, so is this one
        if (classifieds[iprev] == KWARGS_POSITIONAL) {
            classifieds[icurr] = KWARGS_POSITIONAL;
            continue;
        }
        // if previous arg required a value, mark current as KWARGS_VALUE
        if ((classifieds[iprev] == KWARGS_OPTIONAL) || (classifieds[iprev] == KWARGS_REQUIRED)) {
            classifieds[icurr] = KWARGS_VALUE;
            continue;
        }
        const KwargsClass * cls = get_class(argv[icurr], nclasses, classes);
        if (cls == nullptr) {
            classifieds[icurr] = KWARGS_POSITIONAL;
            continue;
        } else {
            classifieds[icurr] = cls->type;
        }
    }
    // assert required args are present
    for (size_t i = 0; i < nclasses; i++) {
        if (classes[i].type != KWARGS_REQUIRED) continue;
        int iarg = has_type(classes[i].longname, argc, argv, nclasses, classes, *nclassifieds, classifieds, KWARGS_REQUIRED);
        if (iarg == 0) {
            fprintf(stderr, "ERROR: Required parameter \"%s\" seems to be missing, aborting.\n", classes[i].longname);
            exit(EXIT_FAILURE);
        }
    }
}


KwargsType * kwargs_create (int argc) {
    errno = 0;
    KwargsType * classifieds = calloc(argc, sizeof(KwargsType));
    if (classifieds == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for array of \"KwargsType\", aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }
    return classifieds;
}


void kwargs_destroy (KwargsType ** classifieds) {
    free(*classifieds);
    *classifieds = nullptr;
}


const char * kwargs_get_optional_value (const char * name,
                                        int argc, const char * argv[],
                                        size_t nclasses, const KwargsClass * classes,
                                        size_t nclassifieds, const KwargsType * classifieds) {
    int iarg = has_type(name, argc, argv, nclasses, classes, nclassifieds, classifieds, KWARGS_OPTIONAL);
    if (iarg == 0) {
        return nullptr;
    }
    if (iarg + 1 >= argc) {
        fprintf(stderr, "Value for \"%s\" seems to be missing, aborting.\n", name);
        exit(EXIT_FAILURE);
    }
    return argv[iarg + 1];
}


const char * kwargs_get_positional_value (size_t ipos,
                                          int argc, const char * argv[],
                                          size_t nclassifieds, const KwargsType * classifieds) {
    assert((size_t) argc == nclassifieds && "Expected number of command line arguments to be the same as number of arguments that were classified");
    size_t k = 0;
    for (size_t i = 0; i < nclassifieds; i++) {
        if (classifieds[i] == KWARGS_POSITIONAL) {
            if (k == ipos) {
                return argv[i];
            } else {
                k++;
            }
        }
    }
    fprintf(stderr, "ERROR: requested positional argument %zu doesn't exist, aborting.\n", ipos);
    exit(EXIT_FAILURE);
}


const char * kwargs_get_required_value (const char * name,
                                        int argc, const char * argv[],
                                        size_t nclasses, const KwargsClass * classes,
                                        size_t nclassifieds, const KwargsType * classifieds) {
    int iarg = has_type(name, argc, argv, nclasses, classes, nclassifieds, classifieds, KWARGS_REQUIRED);
    if (iarg == 0) {
        return nullptr;
    }
    if (iarg + 1 >= argc) {
        fprintf(stderr, "Value for \"%s\" seems to be missing, aborting.\n", name);
        exit(EXIT_FAILURE);
    }
    return argv[iarg + 1];
}


int kwargs_has_flag (const char * name,
                     int argc, const char * argv[],
                     size_t nclasses, const KwargsClass * classes,
                     size_t nclassifieds, const KwargsType * classifieds) {
    return has_type(name, argc, argv, nclasses, classes, nclassifieds, classifieds, KWARGS_FLAG);
}


int kwargs_has_optional (const char * name,
                         int argc, const char * argv[],
                         size_t nclasses, const KwargsClass * classes,
                         size_t nclassifieds, const KwargsType * classifieds) {
    return has_type(name, argc, argv, nclasses, classes, nclassifieds, classifieds, KWARGS_OPTIONAL);
}


void kwargs_print_classifications (FILE * stream,
                                   int argc, const char * argv[],
                                   size_t nclassifieds, const KwargsType * classifieds) {
    assert((size_t) argc == nclassifieds && "Expected number of command line arguments to be the same as number of arguments that were classified");
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
    for (size_t i = 0; i < nclassifieds; i++) {
        int itype = classifieds[i];
        fprintf(stream, "%-*s %s\n", ell, argv[i], typestrings[itype]);
    }
}
