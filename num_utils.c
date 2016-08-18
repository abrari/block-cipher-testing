#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "num_utils.h"

unsigned int array_max(unsigned int *arr, unsigned int length) {
    unsigned int i;
    unsigned int max = arr[0];
    for (i = 1; i < length; ++i) {
        if (max < arr[i]) {
            max = arr[i];
        }
    }
    return max;
}

unsigned int log_2(unsigned int v) {
    // https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious

    unsigned int r = 0; // r will be lg(v)
    while (v >>= 1) // unroll for more speed...
    {
        r++;
    }
    return r;
}

unsigned int nbits(unsigned int i) {
    if (i == 0) return 0;
    return log_2(i) + 1;
}

unsigned int parity(unsigned int v) {
    // https://graphics.stanford.edu/~seander/bithacks.html#ParityParallel

    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v &= 0xf;
    return (0x6996 >> v) & 1u;
}

unsigned int hamming_weight(unsigned int v) {
    // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel

    v = v - ((v >> 1) & 0x55555555);                    // reuse input as temporary
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
    return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
}

double correlation(unsigned int *x, unsigned int *y, unsigned int n) {

    double sx = 0.0;
    double sy = 0.0;
    double sxx = 0.0;
    double syy = 0.0;
    double sxy = 0.0;
    int i;

    for(i = 0; i < n; ++i) {
        double xi = x[i];
        double yi = y[i];

        sx += xi;
        sy += yi;
        sxx += xi * xi;
        syy += yi * yi;
        sxy += xi * yi;
    }

    // covariation
    double cov = sxy / n - sx * sy / n / n;
    if (cov == 0) return 0.0;

    // standard error of x
    double sigmax = sqrt(sxx / n -  sx * sx / n / n);
    // standard error of y
    double sigmay = sqrt(syy / n -  sy * sy / n / n);

    // correlation is just a normalized covariation
    return cov / sigmax / sigmay;
}

// matrix allocate and print

unsigned int **alloc_uint_matrix(unsigned int m, unsigned int n) {
    unsigned int **mat;
    unsigned int i;
    mat = malloc(m * sizeof(unsigned int*));
    for (i = 0; i < m; ++i) {
        mat[i] = calloc(n, sizeof(unsigned int));
    }
    return mat;
}

void print_uint_matrix(unsigned int **mat, unsigned int m, unsigned int n) {

    unsigned int i, j;
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            printf("%2d ", mat[i][j]);
        }
        printf("\n");
    }

}

float **alloc_float_matrix(unsigned int m, unsigned int n) {
    float **mat;
    unsigned int i;
    mat = malloc(m * sizeof(float*));
    for (i = 0; i < m; ++i) {
        mat[i] = calloc(n, sizeof(float));
    }
    return mat;
}

void print_float_matrix(float **mat, unsigned int m, unsigned int n) {

    unsigned int i, j;
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            printf("%5.2f ", mat[i][j]);
        }
        printf("\n");
    }

}