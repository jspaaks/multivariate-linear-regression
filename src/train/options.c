#include "options.h"
#include "kwargs/kwargs.h"
#include <string.h>
#include <stdlib.h>

static const KwargsClass classes[] = {
    {
        .longname = "--device",
        .shortname = "-d",
        .type = KWARGS_OPTIONAL
    },
    {
        .longname = "--learning_rate",
        .shortname = "-r",
        .type = KWARGS_OPTIONAL
    },
    {
        .longname = "--nepochs",
        .shortname = "-e",
        .type = KWARGS_OPTIONAL
    },
    {
        .longname = "--standardize",
        .shortname = "-z",
        .type = KWARGS_FLAG
    },
    {
        .longname = "--verbose",
        .shortname = "-v",
        .type = KWARGS_FLAG
    },
    {
        .longname = "--initial_weights",
        .shortname = "-w",
        .type = KWARGS_OPTIONAL
    }
};


const KwargsClass * options_get_classes (void) {
    return &classes[0];
}


const char * options_get_device (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value ("--device", kwargs);
    if (s == nullptr) {
        return "?";
    }
    return s;
}


const char * options_get_features_path (const Kwargs * kwargs) {
    return kwargs_get_positional_value(0, kwargs);
}


void options_get_initial_weights (const Kwargs * kwargs, Matrix * weights) {
    const char * parameter_name = "--initial_weights";
    char * s = (char *) kwargs_get_optional_value (parameter_name, kwargs);
    if (s == nullptr) {
        for (size_t i = 0; i < weights->n; i++) {
            weights->xs[i] = 0.0f;
        }
        return;
    }
    size_t nfound = 0;
    char * token = nullptr;
    char * next = nullptr;
    for (size_t i = 0; i < weights->n; i++) {
        token = strtok_r(s, ",", &next);
        if (token == nullptr) break;
        char remainder[128] = {};
        int nscanned = sscanf(token, " %f %127s ", &weights->xs[i], remainder);
        if (nscanned != 1) {
            fprintf(stderr, "ERROR: Found trailing characters at index %zu when sscanf'ing "
                            "floating point values from '%s'\n", i, parameter_name);
            exit(EXIT_FAILURE);
        }
        nfound = i + 1;
        s = nullptr;  // (strtok_r magic)
    }
    if (nfound != weights->n) {
        fprintf(stderr, "ERROR: Expected to sscanf %zu floating point values for parameter "
                        "'%s', but found %zu\n", weights->n, parameter_name, nfound);
        exit(EXIT_FAILURE);
    }
}


const char * options_get_labels_path (const Kwargs * kwargs) {
    return kwargs_get_positional_value(1, kwargs);
}


float options_get_learning_rate (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value("--learning_rate", kwargs);
    if (s == nullptr) {
        return (float) 0.01;
    }
    float learning_rate;
    sscanf(s, " %f", &learning_rate);
    return learning_rate;
}


size_t options_get_nclasses (void) {
    return sizeof(classes) / sizeof(classes[0]);
}


size_t options_get_nepochs (const Kwargs * kwargs) {
    const char * s = kwargs_get_optional_value("--nepochs", kwargs);
    if (s == nullptr) {
        return (size_t) 10;
    }
    size_t nepochs;
    sscanf(s, " %zu", &nepochs);
    return nepochs;
}


bool options_get_standardize (const Kwargs * kwargs) {
    return kwargs_has_flag("--standardize", kwargs) > 0;
}


bool options_get_verbose (const Kwargs * kwargs) {
    return kwargs_has_flag("--verbose", kwargs) > 0;
}


void options_show_usage (FILE * stream) {
    char usage[] =
        "NAME\n"
        "       train - Use batch gradient descent to train a multivariate linear model\n"
        "\n"
        "SYNOPSIS\n"
        "       train -h\n"
        "       train --help\n"
        "       train [OPTIONAL]... FEATURES_PATH LABELS_PATH\n"
        "\n"
        "DESCRIPTION\n"
        "       Use batch gradient descent to train a multivariate linear model.\n"
        "\n"
        "       Optionals\n"
        "\n"
        "       -d DEVICE, --device DEVICE\n"
        "            The output device used for plotting. Use \"?\" for interactively\n"
        "            selecting a device, \"null\" for no output. Defaults to interactive\n"
        "            device selection, including overview of available device names.\n"
        "\n"
        "       -e NEPOCHS, --nepochs NEPOCHS\n"
        "            Number of epochs. An epoch is equivalent to one pass through the\n"
        "            entire data set.\n"
        "\n"
        "       -h, --help\n"
        "            Show the help.\n"
        "\n"
        "       -r LEARNING_RATE, --learning_rate LEARNING_RATE\n"
        "            The learning rate of the steepest descent analysis. Simply a\n"
        "            scaling factor that is applied to the calculated gradient of the\n"
        "            loss with respect to the weights.\n"
        "\n"
        "       -v, --verbose\n"
        "            Verbose output.\n"
        "\n"
        "       -w INITIAL_WEIGHTS, --initial_weights INITIAL_WEIGHTS\n"
        "            The point in the parameter space from where to start the iterative\n"
        "            procedure of updating the weights.\n"
        "\n"
        "       -z, --standardize\n"
        "            Whether to standardize the features and labels values before\n"
        "            training.\n"
        "\n"
        "       Positionals\n"
        "\n"
        "       FEATURES_PATH\n"
        "            The path to the file containing the features used for training.\n"
        "            Paths can be relative or absolute. The file format should be that\n"
        "            outputted by sibling program \"mkdata\".\n"
        "\n"
        "       LABELS_PATH\n"
        "            The path to the file containing the labels used for training.\n"
        "            Paths can be relative or absolute. The file format should be that\n"
        "            outputted by sibling program \"mkdata\".\n"
        "\n"
        "EXAMPLES\n"
        "       Train a linear model based on the features from \"features.txt\" and the\n"
        "       labels from \"labels.txt\" using default options:\n"
        "\n"
        "            $ ./train features.txt labels.txt\n"
        "\n"
        "       Train a linear model based on the features from \"data/features.txt\" and\n"
        "       the labels from \"data/labels.txt\" using 100 epochs, a learning rate of\n"
        "       0.001, and initialize the weights at (10.2, 34.7, 0.1); afterwards, use\n"
        "       the interactive \"qtwidget\" device to make a plot of the loss as a\n"
        "       function of epoch:\n"
        "\n"
        "            $ ./train --device qtwidget \\\n"
        "                      --nepochs 100 \\\n"
        "                      --learning_rate 0.001 \\\n"
        "                      --initial_weights 10.2,34.7,0.1 \\\n"
        "                      data/features.txt data/labels.txt\n";
    fprintf(stream, "%s", usage);
}
