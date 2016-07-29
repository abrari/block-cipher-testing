#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "awd.h"
#include "aes/aes.h"

#define ECB 1

int main() {

    srand(time(NULL));
    byte *P = generate_random_bytes(BYTE);
    //print_bytes(P, BYTE);
    //printf("\n");

    byte *key = generate_random_bytes(BYTE);
    //print_bytes(Q, BYTE);
    //printf("\n");

    byte buffer[16];

    AES128_ECB_encrypt(P, key, buffer);
    printf("C1 = ");
    print_bytes(buffer, BYTE);
    printf("\n");

    flip_bit(P, BYTE, 10);

    byte buffer2[16];

    AES128_ECB_encrypt(P, key, buffer2);
    printf("C2 = ");
    print_bytes(buffer2, BYTE);
    printf("\n");

    byte *diff = xor_bytes(buffer, buffer2, BYTE);

    printf("HW = %d", hamming_weight(diff, BYTE));


}