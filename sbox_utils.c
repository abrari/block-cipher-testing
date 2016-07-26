#include <stdlib.h>
#include <stdio.h>
#include "num_utils.h"

unsigned int **sbox_differential_table(unsigned int *sbox, unsigned int m, unsigned int n) {

    /**
     * https://github.com/sagemath/sagelib/blob/master/sage/crypto/mq/sbox.py
     *
     * m and n can be calculated from sbox as follows:
     *     len = array_size(sbox);
     *     max = array_max(sbox, len);
     *     m = log_2(len), n = nbits(max);
     */

    unsigned int **ddt;
    unsigned int nrows = two_power(m);
    unsigned int ncols = two_power(n);
    unsigned int i, si, di;

    ddt = malloc(nrows * sizeof(unsigned int*));
    for (i = 0; i < nrows; ++i) {
        ddt[i] = calloc(ncols, sizeof(unsigned int));
    }

    for (i = 0; i < nrows; ++i) {
        si = sbox[i];
        for (di = 0; di < nrows; ++di) {
            ddt[di][si^sbox[i^di]] += 1;
        }
    }

    return ddt;

}

unsigned int **func_differential_table(unsigned int (*S)(unsigned int), unsigned int m, unsigned int n) {

    /**
     * same as sbox_differential_table, but uses a function instead of a lookup table
     */

    unsigned int **ddt;
    unsigned int nrows = two_power(m);
    unsigned int ncols = two_power(n);
    unsigned int i, si, di;

    ddt = malloc(nrows * sizeof(unsigned int*));
    for (i = 0; i < nrows; ++i) {
        ddt[i] = calloc(ncols, sizeof(unsigned int));
    }

    for (i = 0; i < nrows; ++i) {
        si = S(i);
        for (di = 0; di < nrows; ++di) {
            ddt[di][si^S(i^di)] += 1;
        }
    }

    return ddt;

}

void print_differential_table(unsigned int **ddt, unsigned int m, unsigned int n) {

    unsigned int nrows = two_power(m);
    unsigned int ncols = two_power(n);

    unsigned int i, j;
    for (i = 0; i < nrows; ++i) {
        for (j = 0; j < ncols; ++j) {
            printf("%2d ", ddt[i][j]);
        }
        printf("\n");
    }

}