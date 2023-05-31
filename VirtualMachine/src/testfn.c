#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "cpu.h"



int main(int argc, char *argv[])
{
    assert(argc == 3);

    uint16_t i = atoi(argv[1]);
    int j = atoi(argv[2]);
    while (1)
    {
        printf ("x = %04x/%d width = %u extx = %04x/%04x \n", i, i, j, sgn_ext(i, j), sign_extend(i, j));
        i <<= 1;
        j++;
    }
    return 0;
}