#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "awd.h"

int main() {

    srand(time(NULL));

    unsigned int *awd_array = awd_count_AES(10000, 0, BIT_AES);
    unsigned int *binom = awd_binom_distrib(10000, BIT_AES);
    int i;

    for (i = 0; i < BIT_AES + 1; ++i) {
        printf("%d\t%d\n", i, awd_array[i]);
    }

    double R = awd_resemblance(awd_array, binom, BIT_AES, 10000);
    printf("R = %lf", R);

    byte *b = generate_random_bytes(BYTE_AES);
    print_bytes(b, BYTE_AES);


}