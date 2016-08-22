#ifndef BLOCK_CIPHER_AWD_H
#define BLOCK_CIPHER_AWD_H

#define BIT_AES 128
#define BYTE_AES (BIT_AES >> 3)

typedef unsigned char byte;

// basic bit operations

byte *generate_random_bytes(unsigned int length);
void print_bytes(byte *bytes, unsigned int length);
void flip_bit(byte *bytes, unsigned int length, unsigned int pos);
byte *xor_bytes(byte *b1, byte *b2, unsigned int length);
unsigned int hamming_weight_bytes(byte *b, unsigned int length);

int is_bit_set(byte *b, unsigned int length, unsigned int pos);

// AWD

double *ac_AES(int num_inputs, unsigned int bit_length);
float **sac_AES(int num_inputs, unsigned int bit_length);

unsigned int *awd_binom_distrib(int num_inputs, unsigned int n);
double awd_resemblance(unsigned int *awd_array, unsigned int *awd_binom, unsigned int n, unsigned int num_inputs);

#endif //BLOCK_CIPHER_AWD_H
