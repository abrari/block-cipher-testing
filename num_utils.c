#include <stdlib.h>
#include <stdio.h>
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