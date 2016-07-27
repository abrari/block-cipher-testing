#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "num_utils.h"

unsigned int **alloc_uint_matrix(unsigned int m, unsigned int n) {
    unsigned int **mat;
    unsigned int i;
    mat = malloc(m * sizeof(unsigned int*));
    for (i = 0; i < m; ++i) {
        mat[i] = calloc(n, sizeof(unsigned int));
    }
    return mat;
}

void print_uint_matrix(unsigned int **ddt, unsigned int m, unsigned int n) {

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

// ==============================================================================

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

    ddt = alloc_uint_matrix(nrows, ncols);

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

    ddt = alloc_uint_matrix(nrows, ncols);

    for (i = 0; i < nrows; ++i) {
        si = S(i);
        for (di = 0; di < nrows; ++di) {
            ddt[di][si^S(i^di)] += 1;
        }
    }

    return ddt;

}

unsigned int xor_mask(unsigned int input, unsigned int mask) {
    // because xor result (x1 ^ x2 ^ x3 ...)  = calculating parity of x1x2x3...

    unsigned int masked = input & mask;
    return parity(masked);
}

unsigned int **sbox_linear_approx_table(unsigned int *sbox, unsigned int m, unsigned int n) {

    unsigned int **lat;
    unsigned int nrows = two_power(m);
    unsigned int ncols = two_power(n);
    unsigned int input, inputMask, outputMask;

    lat = alloc_uint_matrix(nrows, ncols);

    for (input = 0; input < nrows; ++input) {
        for (inputMask = 0; inputMask < nrows; ++inputMask) {
            for (outputMask = 0; outputMask < ncols; ++outputMask) {
                if (xor_mask(input, inputMask) == xor_mask(sbox[input], outputMask)) {
                    lat[inputMask][outputMask] += 1;
                }
            }
        }
    }

    return lat;

}