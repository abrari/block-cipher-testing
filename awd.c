#include <stdio.h>
#include <stdlib.h>
#include "awd.h"

byte *generate_random_bytes(unsigned int length) {
    byte *bytes = malloc(length);
    unsigned int i;
    for (i = 0; i < length; ++i) {
        bytes[i] = rand() % 256;
    }
    return bytes;
}

void print_bytes(byte *bytes, unsigned int length) {
    unsigned int i;
    for (i = 0; i < length; ++i) {
        printf("%02x", bytes[i]);
    }
}

void flip_bit(byte *bytes, unsigned int length, unsigned int pos) {
    // flip single bit of `bytes' at position `pos' (from MSB, zero-indexed)
    if (pos < (length * 8)) {
        unsigned int idx = pos / 8;
        unsigned int mask = pos % 8;
        bytes[idx] ^= (128 >> mask);
    }
}

byte *xor_bytes(byte *b1, byte *b2, unsigned int length) {
    byte *result = malloc(length);
    unsigned int i;
    for (i = 0; i < length; ++i) {
        result[i] = b1[i] ^ b2[i];
    }
    return result;
}

unsigned int hamming_weight(byte *b, unsigned int length) {
    unsigned int w = 0;
    unsigned int i, x;
    for (i = 0; i < length; ++i) {
        // http://stackoverflow.com/a/14010273/1198623
        x = b[i];
        x = (x & 0x55) + (x >> 1 & 0x55);
        x = (x & 0x33) + (x >> 2 & 0x33);
        x = (x & 0x0f) + (x >> 4 & 0x0f);
        w += x;
    }
    return w;
}