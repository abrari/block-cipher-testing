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