#include <criterion/criterion.h>
#include "ann/network.h"

Test(ann_network, one){
    int actual = 2 * 3;
    int expected = 6;
    cr_assert(actual == expected);
}
