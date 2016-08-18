#include <stdio.h>
#include <stdlib.h>
#include "num_utils.h"
#include "sbox_utils.h"

// PRESENT sbox (4 x 4)
unsigned int present_sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};

// sample sbox (3 x 3)
unsigned int sample_sbox[8] = {2, 7, 0, 6, 3, 1, 5, 4};

int main() {

    unsigned int m = 3, n = 3;
    unsigned int **ddt = sbox_differential_table(sample_sbox, m, n);

    printf("DDT:\n");
    print_uint_matrix(ddt, two_power(m), two_power(n));

    unsigned int **lat = sbox_linear_approx_table(sample_sbox, m, n);

    printf("LAT:\n");
    print_uint_matrix(lat, two_power(m), two_power(n));

    float *ac = sbox_ac(sample_sbox, m, n);

    printf("AC:\n");
    int i;
    for (i = 0; i < m; ++i) {
        printf(" %.2f", ac[i]);
    }
    printf("\n");

    float **sac = sbox_sac_matrix(sample_sbox, m, n);

    printf("SAC:\n");
    print_float_matrix(sac, m, n);

    free(lat);
    free(ddt);
    free(sac);
    free(ac);

    double bic = sbox_bic(sample_sbox, m, n);
    printf("BIC = %f\n", bic);

    int linearity = sbox_linearity(sample_sbox, m, n);
    printf("Linearity = %d\n", linearity);

    int nonlinearity = sbox_nonlinearity(sample_sbox, m, n);
    printf("Non-Linearity = %d\n", nonlinearity);


    return 0;
}