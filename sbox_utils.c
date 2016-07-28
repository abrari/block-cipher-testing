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

float **sbox_sac_matrix(unsigned int *sbox, unsigned int m, unsigned int n) {

    float **sac = alloc_float_matrix(m, n);
    unsigned int i, j, X, ei, ej, dei;

    for (i = 0; i < m; ++i) {
        ei = two_power(i);
        for (j = 0; j < n; ++j) {
            ej = two_power(j);
            for (X = 0; X < two_power(m); ++X) {
                dei = sbox[X] ^ sbox[X ^ ei];
                sac[i][j] += (dei & ej) >> j; // increment sac[i][j] if bit at position j of dei is set
            }
        }
    }

    float outputLength = (float)two_power(n);
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            sac[i][j] /= outputLength;
        }
    }

    return sac;

}
