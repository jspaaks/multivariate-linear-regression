#include "protected.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>


const KwargsClass * get_class (const char * name, size_t nclasses, const KwargsClass * classes) {
    for (size_t i = 0; i < nclasses; i++) {
        {
            char * longname = classes[i].longname;
            bool matches = longname != nullptr &&
                           strncmp(longname, name, strlen(longname) + 1) == 0;
            if (matches) return &classes[i];
        }
        {
            char * shortname = classes[i].shortname;
            bool matches = shortname != nullptr &&
                           strncmp(shortname, name, strlen(shortname) + 1) == 0;
            if (matches) return &classes[i];
        }
    }
    return nullptr;
}


int has_type (const char * name, int argc, const char * argv[], size_t nclasses, const KwargsClass * classes, size_t nclassifieds, const KwargsType * classifieds, KwargsType type) {
    const char typenames[][20] = {
        "other",
        "exe",
        "flag",
        "optional",
        "positional",
        "required",
        "value"
    };
    assert((size_t) argc == nclassifieds && "Expected number of command line arguments to be the same as number of arguments that were classified");
    const KwargsClass * cls = get_class(name, nclasses, classes);
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
        for (int iarg = 1; iarg < argc; iarg++) {
            bool cond = strncmp(names[iname], argv[iarg], ell) == 0;
            if (cond) {
                // test whether classification is as expected
                if (cls->type != classifieds[iarg]) {
                    fprintf(stdout, "ERROR: Name \"%s\" seems to have been misclassified. Check\n"
                                    "the spelling of parameter names and check whether parameter\n"
                                    "names that require a value did in fact get one.\n", name);
                    exit(EXIT_FAILURE);
                }
                return iarg;
            }
        }
    }
    return 0;
}
