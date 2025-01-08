#include "protected.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


void assert_no_duplicate_names (Kwargs * kwargs) {
    fprintf(stdout, "WIP");
    size_t nclasses = kwargs->nclasses;
    size_t ncap = nclasses * 2;
    char ** seen = calloc(ncap, sizeof(char *));
    if (seen == nullptr) {
        fprintf(stderr, "ERROR: Problem allocating memory for array \"seen\", aborting.\n");
        exit(EXIT_FAILURE);
    }
    size_t nseen = 0;
    for (size_t icls = 0; icls < nclasses; icls++) {
        {
            char * name = kwargs->classes[icls].shortname;
            for (size_t iseen = 0; iseen < nseen; iseen++) {
                bool matches = strncmp(name, seen[iseen], 3) == 0;
                if (matches) {
                    fprintf(stderr, "ERROR: duplicate name \"%s\" found, aborting.\n", name);
                    exit(EXIT_FAILURE);
                }
            }
            seen[nseen] = name;
            nseen++;
        }
        {
            char * name = kwargs->classes[icls].longname;
            for (size_t iseen = 0; iseen < nseen; iseen++) {
                bool matches = strncmp(name, seen[iseen], 65) == 0;
                if (matches) {
                    fprintf(stderr, "ERROR: duplicate name \"%s\" found, aborting.\n", name);
                    exit(EXIT_FAILURE);
                }
            }
            seen[nseen] = name;
            nseen++;
        }
    }
    assert(nseen == ncap && "Expected number of seen names to be equal to number of allocated items");
}


void assert_no_unnameds (Kwargs * kwargs) {
    for (size_t i = 0; i < kwargs->nclasses; i++) {
        bool a = kwargs->classes[i].shortname == nullptr;
        bool b = kwargs->classes[i].longname == nullptr;
        if (a && b) {
            fprintf(stderr, "ERROR: Found KwargsClass instance without a shortname and without\n"
                            "a longname, aborting.\n");
            exit(EXIT_FAILURE);
        }
    }
}


void assert_shortnames_are_compliant (Kwargs * kwargs) {
    for (size_t i = 0; i < kwargs->nclasses; i++) {
        bool present = kwargs->classes[i].shortname != nullptr;
        if (!present) continue;
        if (strnlen(kwargs->classes[i].shortname, 3) != 2) {
            fprintf(stderr, "ERROR: shortname \"%s\" should be length 2, aborting.\n", kwargs->classes[i].shortname);
            exit(EXIT_FAILURE);
        }
        if (kwargs->classes[i].shortname[0] != '-') {
            fprintf(stderr, "ERROR: shortname \"%s\" should start with \'-\', aborting.\n", kwargs->classes[i].shortname);
            exit(EXIT_FAILURE);
        }
        if (!isalpha(kwargs->classes[i].shortname[1])) {
            fprintf(stderr, "ERROR: shortname \"%s\" character at index 1 should\n"
                            "be [a-zA-Z], aborting.\n", kwargs->classes[i].shortname);
            exit(EXIT_FAILURE);
        }
    }
}


void assert_longnames_are_compliant (Kwargs * kwargs) {
    for (size_t i = 0; i < kwargs->nclasses; i++) {
        bool present = kwargs->classes[i].longname != nullptr;
        if (!present) continue;
        if (strnlen(kwargs->classes[i].longname, 4) <= 3) {
            fprintf(stderr, "ERROR: longname \"%s\" should be at least 4 characters, aborting.\n", kwargs->classes[i].longname);
            exit(EXIT_FAILURE);
        }
        if (strnlen(kwargs->classes[i].longname, 65) > 64) {
            fprintf(stderr, "ERROR: longname \"%s\" should be at most 64 characters, aborting.\n", kwargs->classes[i].longname);
            exit(EXIT_FAILURE);
        }
        if (kwargs->classes[i].longname[0] != '-') {
            fprintf(stderr, "ERROR: longname \"%s\" should start with \"--\", aborting.\n", kwargs->classes[i].longname);
            exit(EXIT_FAILURE);
        }
        if (kwargs->classes[i].longname[1] != '-') {
            fprintf(stderr, "ERROR: longname \"%s\" should start with \"--\", aborting.\n", kwargs->classes[i].longname);
            exit(EXIT_FAILURE);
        }
    }
}


void assert_types_are_correct_subset (Kwargs * kwargs) {
    for (size_t i = 0; i < kwargs->nclasses; i++) {
        bool a = kwargs->classes[i].type == KWARGS_FLAG;
        bool b = kwargs->classes[i].type == KWARGS_OPTIONAL;
        bool c = kwargs->classes[i].type == KWARGS_REQUIRED;
        bool compliant = a || b || c;
        if (!compliant) {
            fprintf(stderr, "ERROR: all parameter types should be one of [KWARGS_FLAG,\n"
                            "KWARGS_OPTIONAL, KWARGS_REQUIRED], aborting.\n");
            exit(EXIT_FAILURE);
        }
    }
}


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

    assert_no_unnameds(kwargs);
    assert_shortnames_are_compliant(kwargs);
    assert_longnames_are_compliant(kwargs);
    assert_types_are_correct_subset (kwargs);
    assert_no_duplicate_names(kwargs);

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
