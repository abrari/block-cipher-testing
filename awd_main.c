#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "awd.h"
#include "num_utils.h"

int main() {

    srand(time(NULL));

    unsigned int *awd_array = awd_count_AES(10000, 125, BIT_AES);
    unsigned int *awd_binom = awd_binom_distrib(10000, BIT_AES);
    int i;
    for (i = 0; i < BIT_AES; ++i) {
        printf("%d\t%d\t%d\n", i, awd_array[i], awd_binom[i]);
    }

}