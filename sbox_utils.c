#include "num_utils.h"

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

unsigned int **sbox_linear_approx_table(unsigned int *sbox, unsigned int m, unsigned int n) {

    unsigned int **lat;
    unsigned int nrows = two_power(m);
    unsigned int ncols = two_power(n);
    unsigned int input, inputMask, outputMask;

    lat = alloc_uint_matrix(nrows, ncols);

    for (input = 0; input < nrows; ++input) {
        for (inputMask = 0; inputMask < nrows; ++inputMask) {
            for (outputMask = 0; outputMask < ncols; ++outputMask) {
                // x1 ^ x2 ^ x3 ...  = parity of x1x2x3...
                if (parity(input & inputMask) == parity(sbox[input] & outputMask)) {
                    lat[inputMask][outputMask] += 1;
                }
            }
        }
    }

    return lat;

}