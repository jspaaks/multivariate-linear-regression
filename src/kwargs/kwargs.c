#include "kwargs.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void assert_required_args_present (void);
const RecognizedKey * get_parent (char * actual_arg, size_t nkeys, const RecognizedKey * recognized_keys);
void mark_args (int argc, char * argv[], size_t nkeys, const RecognizedKey * recognized_keys, struct marked * marked_args);
bool requires_value (struct marked * marked_arg);

// TODO add quality control on recognized_keys, such as
//     no duplicate names
//     not bot longname and shortname nullptrs


void assert_required_args_present (void) {
}


bool requires_value (struct marked * marked_arg) {
    if (marked_arg->argtype == KWARGS_OPTIONAL &&
        marked_arg->parent != nullptr &&
        marked_arg->parent->takesvalue) return true;

    if (marked_arg->argtype == KWARGS_REQUIRED &&
        marked_arg->parent != nullptr &&
        marked_arg->parent->takesvalue) return true;

    return false;
}


const RecognizedKey * get_parent (char * actual_arg, size_t nkeys, const RecognizedKey * recognized_keys) {
    for (size_t i = 0; i < nkeys; i++) {
        char * longname = recognized_keys[i].longname;
        char * shortname = recognized_keys[i].shortname;
        bool longname_matches = longname != nullptr && strncmp(actual_arg, longname, strlen(longname)) == 0;
        bool shortname_matches = shortname != nullptr && strncmp(actual_arg, shortname, strlen(shortname)) == 0;
        if (longname_matches || shortname_matches) return &recognized_keys[i];
    }
    return nullptr;
}


void mark_args (int argc, char * argv[], size_t nkeys, const RecognizedKey * recognized_keys, struct marked * marked_args) {
    marked_args[0] = (struct marked) {
        .parent = nullptr,
        .argtype = KWARGS_EXENAME
    };
    for (int iarg = 1; iarg < argc; iarg++) {
        char * arg = argv[iarg];
        // if previous arg was positional, so is this one
        if (marked_args[iarg - 1].argtype == KWARGS_POSITIONAL) {
            marked_args[iarg] = (struct marked) {
                .parent = nullptr,
                .argtype = KWARGS_POSITIONAL
            };
            continue;
        }
        // if previous arg required a value, mark current as KWARGS_VALUE
        if (requires_value(&marked_args[iarg - 1])) {
            marked_args[iarg] = (struct marked) {
                .parent = nullptr,
                .argtype = KWARGS_VALUE
            };
            continue;
        }
        const RecognizedKey * parent = get_parent(arg, nkeys, recognized_keys);
        if (parent == nullptr) {
            marked_args[iarg] = (struct marked) {
                .parent = nullptr,
                .argtype = KWARGS_POSITIONAL
            };
        } else {
            if (parent->isrequired && parent->takesvalue) {
                marked_args[iarg] = (struct marked) {
                    .parent = parent,
                    .argtype = KWARGS_REQUIRED
                };
            } else  if (!parent->isrequired && parent->takesvalue) {
                marked_args[iarg] = (struct marked) {
                    .parent = parent,
                    .argtype = KWARGS_OPTIONAL
                };
            } else if (!parent->isrequired && !parent->takesvalue) {
                marked_args[iarg] = (struct marked) {
                    .parent = parent,
                    .argtype = KWARGS_FLAG
                };
            }
        }
    }
}


void marked_args_print (int argc, char * argv[], struct marked * marked_args) {
    for (int i = 0; i < argc; i++) {
        switch (marked_args[i].argtype) {
            case KWARGS_EXENAME: {
                fprintf(stdout, "%s exe\n", argv[i]);
                break;
            }
            case KWARGS_OPTIONAL: {
                char * longname = marked_args[i].parent->longname;
                char * shortname = marked_args[i].parent->shortname;
                fprintf(stdout, "%-12s optional, instance of \"%s\"\n", argv[i], longname == nullptr ? shortname : longname);
                break;
            }
            case KWARGS_REQUIRED: {
                char * longname = marked_args[i].parent->longname;
                char * shortname = marked_args[i].parent->shortname;
                fprintf(stdout, "%-12s required, instance of \"%s\"\n", argv[i], longname == nullptr ? shortname : longname);
                break;
            }
            case KWARGS_FLAG: {
                fprintf(stdout, "%-12s flag\n", argv[i]);
                break;
            }
            case KWARGS_VALUE: {
                fprintf(stdout, "%-12s value of \"%s\"\n", argv[i], argv[i - 1]);
                break;
            }
            case KWARGS_POSITIONAL: {
                fprintf(stdout, "%-12s positional value\n", argv[i]);
                break;
            }
            default: {
                fprintf(stdout, "%-12s unknown\n", argv[i]);
                break;
            }
        }
    }
}


int main (int argc, char * argv[]) {
    constexpr size_t nkeys = 3;
    const RecognizedKey recognized_keys[nkeys] = {
        (RecognizedKey) {
            .isrequired = true,
            .longname = "--nfeatures",
            .shortname = "-d",
            .takesvalue = true
        },
        (RecognizedKey) {
            .isrequired = true,
            .longname = nullptr,
            .shortname = "-n",
            .takesvalue = true
        },
        (RecognizedKey) {
            .isrequired = false,
            .longname = "--help",
            .shortname = "-h",
            .takesvalue = false
        }
    };

    errno = 0;
    struct marked * marked_args = malloc(argc * sizeof(struct marked));
    if (marked_args == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for \"marked_args\" instance, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }
    mark_args(argc, argv, nkeys, recognized_keys, marked_args);
    marked_args_print(argc, argv, marked_args);

    free(marked_args);
    marked_args = nullptr;
    exit(EXIT_SUCCESS);
}
