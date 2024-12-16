#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "data/data.h"
#include "lfuns/lfuns.h"
#include "ops/ops.h"

void print_usage (FILE *, char * []);


int main (int argc, char * argv[]) {

    // ========================= PARSE ARGUMENTS ========================== //

    if (argc == 1) {
        print_usage(stderr, argv);
        exit(EXIT_FAILURE);
    }
    {
        bool a = strncmp(argv[1], "-h", 3) == 0;
        bool b = strncmp(argv[1], "--help", 7) == 0;
        if (a || b) {
            print_usage(stdout, argv);
            exit(EXIT_SUCCESS);
        }
    }

    // ============================= READ DATA ============================ //

    const char * images_path = argv[1];
    const char * labels_path = argv[2];
    Matrix * images = data_read_images(images_path);
    Matrix * labels = data_read_labels(labels_path);

    // =================== INITIALIZE RANDOMIZATION ======================= //

    srand(time(nullptr));

    // ======================= INITIALIZE ARRAYS ========================== //

    Matrix * w1 = data_create(300, images->nr);
    Matrix * hidden1 = data_create(w1->nr, images->nc);
    Matrix * w2 = data_create(100, hidden1->nc);
    Matrix * hidden2 = data_create(w2->nr, hidden1->nc);
    Matrix * w3 = data_create(10, hidden2->nc);
    Matrix * output = data_create(w3->nr, hidden2->nc);

    float loss = 0.0f;

    //// ========================== FORWARD PASS ============================ //

    ops_dot_product(w1, images, hidden1);
    ops_dot_product(w2, hidden1, hidden2);
    ops_dot_product(w3, hidden2, output);
    ops_svm(output, labels, &loss);

    //// ========================= BACKWARD PASS ============================ //

    //// TODO

    //// =================== DEALLOCATE DYNAMIC MEMORY ====================== //

    data_destroy(&output);
    data_destroy(&w3);
    data_destroy(&hidden2);
    data_destroy(&w2);
    data_destroy(&hidden1);
    data_destroy(&w1);
    data_destroy(&labels);
    data_destroy(&images);

    return EXIT_SUCCESS;
}


void print_usage(FILE * stream, char * argv[]) {
    fprintf(stream,
            "Usage: %s IMAGES_PATH LABELS_PATH\n"
            "   Read IDX formatted images from IMAGES_PATH and read the\n"
            "   corresponding IDX formatted labels from LABELS_PATH.\n"
            "   Subsequently use these data to train a neural network.\n", argv[0]);
}
