#include <criterion/criterion.h>
#include "network/network.h"

Test(network, one){
    int actual = 2 * 3;
    int expected = 6;
    cr_assert(actual == expected);
}
