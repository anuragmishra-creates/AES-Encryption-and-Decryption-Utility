#ifndef AES_ENCRYPTION_H
#define AES_ENCRYPTION_H

#include <stdint.h>

void shiftRows(uint8_t array[4][4]);
void subBytes(uint8_t word[4][4]);
void mixColumns(uint8_t state[4][4]);
void encryptAES(char *txt, uint8_t roundKeys[15][4][4], int modeOfOperationIndex);

#endif

