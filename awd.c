#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "awd.h"
#include "aes/aes.h"
#include "num_utils.h"

#define ECB 1

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

int is_bit_set(byte *b, unsigned int length, unsigned int pos) {

    unsigned int idx = pos / 8;
    unsigned int realpos = pos - (idx * 8);

    return (b[idx] & (128 >> realpos)) ? 1 : 0;

}

unsigned int hamming_weight_bytes(byte *b, unsigned int length) {
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

/**
 * Calculate AWD distribution for AES
 * Kavut & Yucel: "On Some Cryptographic Properties of Rijndael"
 *
 * @param num_inputs    Number of plaintexts to generate the distribution (10000)
 * @param bit_flip_pos  Which plaintext bit to flip (0-127)
 * @param bit_length    Cipher bit length (128 for AES)
 */
double *ac_AES(int num_inputs, unsigned int bit_length) {

    double *k_aval = calloc(bit_length, sizeof(double));
    unsigned int byte_length = bit_length / 8u;
    unsigned int i;
    unsigned int w;
    unsigned int ei;

    byte *key = generate_random_bytes(byte_length);
    byte *C  = malloc(byte_length);
    byte *Ci = malloc(byte_length);

    for (ei = 0; ei < bit_length; ++ei) {
        printf("Flipping bit %d...\n", ei);
        for (i = 0; i < num_inputs; ++i) {
            byte *P = generate_random_bytes(byte_length);

            AES128_ECB_encrypt(P, key, C);
            flip_bit(P, byte_length, ei);
            AES128_ECB_encrypt(P, key, Ci);

            byte *Dei = xor_bytes(C, Ci, byte_length);
            w = hamming_weight_bytes(Dei, byte_length);

            k_aval[ei] += w;

            free(P);
            free(Dei);
        }
    }

    float div = (float)num_inputs * bit_length;
    for (ei = 0; ei < bit_length; ++ei) {
        k_aval[ei] /= div;
    }

    free(C);
    free(Ci);
    free(key);

    return k_aval;

}

float **sac_AES(int num_inputs, unsigned int bit_length) {

    float **sac = alloc_float_matrix(bit_length, bit_length);
    unsigned int byte_length = bit_length / 8u;
    unsigned int i;
    unsigned int ei, ej;

    byte *key = generate_random_bytes(byte_length);
    byte *C  = malloc(byte_length);
    byte *Ci = malloc(byte_length);

    for (ei = 0; ei < bit_length; ++ei) {
        // printf("Flipping bit %d...\n", ei);
        for (i = 0; i < num_inputs; ++i) {
            byte *P = generate_random_bytes(byte_length);

            AES128_ECB_encrypt(P, key, C);
            flip_bit(P, byte_length, ei);
            AES128_ECB_encrypt(P, key, Ci);

            byte *Dei = xor_bytes(C, Ci, byte_length);

            for (ej = 0; ej < bit_length; ++ej) {
                sac[ei][ej] += is_bit_set(Dei, byte_length, ej);
            }
            free(Dei);
            free(P);
        }
    }

    float div = (float)num_inputs;
    for (ei = 0; ei < bit_length; ++ei) {
        for (ej = 0; ej < bit_length; ++ej) {
            sac[ei][ej] /= div;
        }
    }

    free(C);
    free(Ci);
    free(key);

    return sac;

}


/**
 * Calculate the ideal binomial distribution
 *
 * @param num_inputs    Equals awd_count_AES's num_inputs
 * @param n             Equals awd_count_AES's bit_length
 */
unsigned int *awd_binom_distrib(int num_inputs, unsigned int n) {

    unsigned int *B = calloc(n, sizeof(unsigned int));
    unsigned int i;

    // calculated from Excel BINOM.DIST(i, n, 0.5, false)
    // because the combination(n, k) will overflow in C unless we use GMP / bignum library
    double prob[128] = {
        2.93873587705572E-039,
        3.76158192263132E-037,
        2.38860452087089E-035,
        1.00321389876577E-033,
        3.13504343364304E-032,
        7.77490771543474E-031,
        1.59385608166412E-029,
        2.77786345661461E-028,
        4.2015184781296E-027,
        5.60202463750613E-026,
        6.6664093186323E-025,
        7.1512390872601E-024,
        6.9724581100786E-023,
        6.22157800591629E-022,
        5.11058193343124E-021,
        3.88404226940774E-020,
        2.74310485276922E-019,
        1.80722202064795E-018,
        1.11445357939957E-017,
        6.45209967020805E-017,
        3.51639432026339E-016,
        1.80843136470688E-015,
        8.79555254652894E-015,
        4.05360247796551E-014,
        1.77345108410991E-013,
        7.37755650989723E-013,
        2.92264738661313E-012,
        1.10411123494274E-011,
        3.98268695461488E-011,
        1.37334032917755E-010,
        4.5320230862859E-010,
        1.43270407243877E-009,
        4.34288421958002E-009,
        1.26338450024146E-008,
        3.53004492714525E-008,
        9.48069209004725E-008,
        2.44917878992887E-007,
        0.000000609,
        1.45835880720374E-006,
        3.36544340123939E-006,
        7.48811156775765E-006,
        0.000016072,
        3.32920918482884E-005,
        6.65841836965767E-005,
        0.0001286285,
        0.0002401066,
        0.0004332358,
        0.0007558582,
        0.0012755108,
        0.0020824666,
        0.0032902972,
        0.0050322193,
        0.0074515555,
        0.0106852494,
        0.0148406241,
        0.0199673852,
        0.0260289129,
        0.0328786268,
        0.0402479742,
        0.0477518338,
        0.0549146088,
        0.0612162852,
        0.0661530824,
        0.0693032292,
        0.0703860922,
        0.0693032292,
        0.0661530824,
        0.0612162852,
        0.0549146088,
        0.0477518338,
        0.0402479742,
        0.0328786268,
        0.0260289129,
        0.0199673852,
        0.0148406241,
        0.0106852494,
        0.0074515555,
        0.0050322193,
        0.0032902972,
        0.0020824666,
        0.0012755108,
        0.0007558582,
        0.0004332358,
        0.0002401066,
        0.0001286285,
        6.65841836965767E-005,
        3.32920918482883E-005,
        0.000016072,
        7.48811156775765E-006,
        3.36544340123939E-006,
        1.45835880720374E-006,
        0.000000609,
        2.44917878992887E-007,
        9.48069209004725E-008,
        3.53004492714525E-008,
        1.26338450024146E-008,
        4.34288421958002E-009,
        1.43270407243877E-009,
        4.5320230862859E-010,
        1.37334032917755E-010,
        3.98268695461488E-011,
        1.10411123494274E-011,
        2.92264738661313E-012,
        7.37755650989723E-013,
        1.77345108410991E-013,
        4.05360247796551E-014,
        8.79555254652894E-015,
        1.80843136470688E-015,
        3.51639432026339E-016,
        6.45209967020805E-017,
        1.11445357939957E-017,
        1.80722202064795E-018,
        2.74310485276922E-019,
        3.88404226940774E-020,
        5.11058193343124E-021,
        6.22157800591629E-022,
        6.9724581100786E-023,
        7.1512390872601E-024,
        6.6664093186323E-025,
        5.60202463750614E-026,
        4.2015184781296E-027,
        2.77786345661461E-028,
        1.59385608166412E-029,
        7.77490771543474E-031,
        3.13504343364304E-032,
        1.00321389876577E-033,
        2.38860452087089E-035,
        3.76158192263132E-037
    };

    for (i = 0; i < n; ++i) {
        B[i] = round(prob[i] * num_inputs);
    }

    return B;

}

/**
 * How close the AWD of the cipher from the ideal binomial distrib
 * Arikan: "PROPAGATION CHARACTERISTICS OF RC5, RC6 AND TWOFISH CIPHERS"
 */
double awd_resemblance(unsigned int *awd_array, unsigned int *awd_binom, unsigned int n, unsigned int num_inputs) {

    unsigned int i;
    double r = 0.0, d = 0.0, errorsum = 0.0;
    for (i = 0; i < n; ++i) {
        errorsum += abs(awd_array[i] - awd_binom[i]);
    }
    d = (1.0 / (2 * num_inputs)) * errorsum;
    if (d > 1.0) d = 1.0;
    if (d < 0.0) d = 0.0;
    r = 1 - d;

    return r;

}




