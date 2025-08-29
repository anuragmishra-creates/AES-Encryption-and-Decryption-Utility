#ifndef AES_KEYSCHEDULE_H
#define AES_KEYSCHEDULE_H

#include <stdint.h>

uint32_t rotWord(uint32_t word);
uint32_t subWord(uint32_t word);
uint32_t rCon(uint32_t word, int round);
uint32_t setByte(int byte0, int byte1, int byte2, int byte3);
void keySchedule(uint8_t roundKeys[15][4][4], uint8_t cipherKey[4][8]);


#endif