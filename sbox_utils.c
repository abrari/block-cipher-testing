#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

    // the LAT is in original values (not subtracted by 2^n / 2)

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

int sbox_linearity(unsigned int *sbox, unsigned int m, unsigned int n) {

    // https://www.cosic.esat.kuleuven.be/ecrypt/courses/mykonos12/slides/day2/gl.pdf#59

    unsigned int **lat = sbox_linear_approx_table(sbox, m, n);
    unsigned int nrows = two_power(m);
    unsigned int ncols = two_power(n);
    unsigned int i, j;
    int val, maxAbs = 0;

    // skip zero input/output mask
    for (i = 1; i < nrows; ++i) {
        for (j = 1; j < ncols; ++j) {
            val = abs(lat[i][j] - (two_power(m) / 2));
            if (maxAbs < val)
                maxAbs = val;
        }
    }

    free(lat);

    return maxAbs;

}

int sbox_nonlinearity(unsigned int *sbox, unsigned int m, unsigned int n) {

    // the easy way
    // - https://github.com/okazymyrov/sbox/blob/master/Sage/CSbox.sage#L842
    // - http://crypto.stackexchange.com/a/19976

    int lin = sbox_linearity(sbox, m, n);
    return two_power(m - 1) - lin;

}

float *sbox_ac(unsigned int *sbox, unsigned int m, unsigned int n) {

    float *kaval = calloc(m, sizeof(float));
    unsigned int i, X, ei, dei, w;

    for (i = 0; i < m; i++) {
        ei = two_power(i);
        for (X = 0; X < two_power(m); ++X) {
            dei = sbox[X] ^ sbox[X ^ ei];
            w   = hamming_weight(dei);
            kaval[i] += w;
        }
    }

    float div = (float)two_power(m) * m;
    for (i = 0; i < m; ++i) {
        kaval[i] /= div;
    }

    return kaval;

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

double sbox_bic(unsigned int *sbox, unsigned int m, unsigned int n) {

    unsigned int i, ei, X, dei;
    unsigned int j, k, ej, ek, dej, dek;
    unsigned int *aval_vector_j;
    unsigned int *aval_vector_k;
    double corr = 0.0;
    double maxCorr = 0.0;

    // for each input bit position
    for (i = 0; i < m; ++i) {
        ei = two_power(i);

        // for each j, k output bit change if j != k
        for (j = 0; j < n; ++j) {
            for (k = 0; k < n; ++k) {
                if (j != k) {
                    aval_vector_j = calloc(two_power(m), sizeof(unsigned int));
                    aval_vector_k = calloc(two_power(m), sizeof(unsigned int));

                    // for each possible input
                    for (X = 0; X < two_power(m); ++X) {
                        ej = two_power(j);
                        ek = two_power(k);

                        dei = sbox[X] ^ sbox[X ^ ei];
                        dej = (dei & ej) >> j;
                        dek = (dei & ek) >> k;

                        aval_vector_j[X] = dej;
                        aval_vector_k[X] = dek;
                    }

                    corr = fabs(correlation(aval_vector_j, aval_vector_k, two_power(m)));
                    // printf("corr[%d][%d] = %f\n", j, k, corr);
                    if (maxCorr < corr)
                        maxCorr = corr;

                    free(aval_vector_j);
                    free(aval_vector_k);
                }
            }
        }
    }

    return maxCorr;

}




