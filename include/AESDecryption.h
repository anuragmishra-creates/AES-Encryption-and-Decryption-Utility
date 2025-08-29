#ifndef AES_DECRYPTION_H
#define AES_DECRYPTION_H

#include <stdint.h>

void inverseShiftRows(uint8_t array[4][4]);
void inverseSubBytes(uint8_t word[4][4]);
void inverseMixColumns(uint8_t state[4][4]);
void decryptAES(char *txt, uint8_t roundKeys[15][4][4], int modeOfOperationIndex);

#endif