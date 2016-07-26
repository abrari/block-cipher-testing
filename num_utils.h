#ifndef BLOCK_CIPHER_NUM_UTILS_H
#define BLOCK_CIPHER_NUM_UTILS_H

#define two_power(n) (1u << (n))

#define array_size(a) (sizeof(a) / sizeof(*a))

unsigned int array_max(unsigned int *, unsigned int);

unsigned int log_2(unsigned int);

unsigned int nbits(unsigned int);

#endif //BLOCK_CIPHER_NUM_UTILS_H
