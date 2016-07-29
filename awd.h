#ifndef BLOCK_CIPHER_AWD_H
#define BLOCK_CIPHER_AWD_H

#define BIT 128
#define BYTE (BIT >> 3)

typedef unsigned char byte;

// basic bit operations

byte *generate_random_bytes(unsigned int length);
void print_bytes(byte *bytes, unsigned int length);
void flip_bit(byte *bytes, unsigned int length, unsigned int pos);
byte *xor_bytes(byte *b1, byte *b2, unsigned int length);
unsigned int hamming_weight(byte *b, unsigned int length);

#endif //BLOCK_CIPHER_AWD_H
