#ifndef BLOCK_CIPHER_SBOX_UTILS_H
#define BLOCK_CIPHER_SBOX_UTILS_H

unsigned int **sbox_differential_table(unsigned int *, unsigned int, unsigned int);

unsigned int **sbox_linear_approx_table(unsigned int *, unsigned int, unsigned int);

float **sbox_sac_matrix(unsigned int *, unsigned int, unsigned int);

#endif //BLOCK_CIPHER_SBOX_UTILS_H
