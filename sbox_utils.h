#ifndef BLOCK_CIPHER_SBOX_UTILS_H
#define BLOCK_CIPHER_SBOX_UTILS_H

unsigned int **sbox_differential_table(unsigned int *, unsigned int, unsigned int);

unsigned int **func_differential_table(unsigned int (*S)(unsigned int), unsigned int, unsigned int);

void print_uint_matrix(unsigned int **, unsigned int, unsigned int);

#endif //BLOCK_CIPHER_SBOX_UTILS_H
