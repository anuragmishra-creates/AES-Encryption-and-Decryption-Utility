#ifndef AES_CONSTANTS_H
#define AES_CONSTANTS_H

#include <stdint.h>

extern const uint8_t sbox[16][16];
extern const uint8_t inverseSBox[16][16];
extern const uint8_t roundConstants[11];

#endif