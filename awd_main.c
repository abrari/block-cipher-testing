#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "awd.h"

int main() {

    srand(time(NULL));
    int k;

    for (k = 0; k < 5; ++k) {

        printf("Run %d: ", k);
        fflush(stdout);

        float **sac = sac_AES(10000, BIT_AES);
        double error, maxError = 0;
        int i, j;

        for (i = 0; i < BIT_AES; ++i) {
            for (j = 0; j < BIT_AES; ++j) {
                error = fabs(sac[i][j] - 0.5);
                if (error > maxError)
                    maxError = error;
            }
        }

        printf("Max Error = %lf\n", maxError);
        fflush(stdout);

        free(sac);
    }


//    double *k_aval = ac_AES(10000, BIT_AES);
//    double error, maxError = 0;
//
//    int i;
//    for (i = 0; i < BIT_AES; ++i) {
//        error = fabs(k_aval[i] - 0.5);
//        if (error > maxError)
//            maxError = error;
//
//        // printf("%f\n", k_aval[i]);
//    }
//
//    printf("Max Error = %lf", maxError);


}