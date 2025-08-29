#ifndef AES_HELPERS_H
#define AES_HELPERS_H

#include <stdint.h>
#include <stdbool.h>

void printSeparator();
void clearBuffer();
void reverse(uint8_t array[], int begin, int end);
void blockGenerate(char txt[], int txtsize, uint8_t stateMatrices[][4][4], int totalBlocks, bool leaveBlock0);
char *lineGenerate(uint8_t stateMatrices[][4][4], int totalBlocks, bool IVpresent);
int hexCharToValue(char c);
void charStringToHexString(char txt[], uint8_t hexstr[], int hexstrsize);
void printBlocks(uint8_t stateMatrices[][4][4], int totalBlocks, bool charFormat);
uint8_t multiply(uint8_t a, uint8_t b);
void XORBintoA(uint8_t A[4][4], uint8_t B[4][4]);
void addRoundKey(uint8_t stateMatrices[4][4], uint8_t roundKey[4][4]);
char *inputString();

#endif