#include "../include/AESEncryption.h"
#include "../include/AESConstants.h"
#include "../include/AESHelpers.h"
#include "../include/AESGlobals.h"
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Encryption: Left Circular shifting of row'th indexed row 'row' times
void shiftRows(uint8_t array[4][4]) // A 4x4 array is received
{
    for (int row = 0; row < 4; row++)
    {
        reverse(array[row], 0, row);
        reverse(array[row], row, 4);
        reverse(array[row], 0, 4);
    }
}

// Encryption: Substitutes the bytes of the word using sbox
void subBytes(uint8_t word[4][4])
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            // Hexadecimal digit of the form 0x(XY)
            int Y = word[row][col] & ((1 << 4) - 1);
            int X = word[row][col] >> 4;
            word[row][col] = sbox[X][Y];
        }
    }
}

// For Encryption: Multiplies the state (temporary stage of a block being encrypted) with a fixed matrix
void mixColumns(uint8_t state[4][4])
{
    uint8_t matrix[4][4] =
        {
            {0x02, 0x03, 0x01, 0x01},
            {0x01, 0x02, 0x03, 0x01},
            {0x01, 0x01, 0x02, 0x03},
            {0x03, 0x01, 0x01, 0x02}};

    uint8_t result[4][4];

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            result[row][col] =
                multiply(matrix[row][0], state[0][col]) ^
                multiply(matrix[row][1], state[1][col]) ^
                multiply(matrix[row][2], state[2][col]) ^
                multiply(matrix[row][3], state[3][col]);
        }
    }
    // Copy result back to state matrix
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            state[row][col] = result[row][col];
        }
    }
}

// For Encryption : The function recieves the text and applies all encryption functions in
//                  the right order to encrypt the text given by the user
void encryptAES(char *txt, uint8_t roundKeys[15][4][4], int modeOfOperationIndex)
{
    bool IVpresent = (modeOfOperationIndex == 2 || modeOfOperationIndex == 3);
    int keySizeInBytes = keySizeInBits / 8;
    int Nk = keySizeInBytes / 4; // Number of words in the cipher key
    int totalRounds = Nk + 6;    // AES standard: Rounds = Nk + 6

    int size = strlen(txt);
    int totalBlocks = (size + 15) / 16; // Basically, ceil(size/16.0);
    if (IVpresent)                      // Extra space for IV
        totalBlocks++;
    uint8_t cipherTextBlocks[totalBlocks][4][4]; // Dynamic: The name refers to the final state it will reach
    uint8_t plainTextBlocks[totalBlocks][4][4];  // Static

    // If required in the current mode of operation, generating IV and storing at cipherTextBlocks[0]
    // and storing 0 matrix at plainTextBlocks[0]=0
    if (IVpresent)
    {
        srand((unsigned int)time(NULL));
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
                cipherTextBlocks[0][row][col] = rand() % 256, plainTextBlocks[0][row][col] = 0;
        }
    }

    // Breaking the text content to blocks each of size 4x4
    blockGenerate(txt, size, plainTextBlocks, totalBlocks, IVpresent);  // plainTextBlocks[0]=0 matrix (if IV is used)
    blockGenerate(txt, size, cipherTextBlocks, totalBlocks, IVpresent); // cipherTextBlocks[0]=IV matrix (if IV is used)

    // Printing the cipherTextBlocks containing the text before encryption
    if (debugMode)
    {
        printf("\n======INDIVIDUAL BLOCKS (DIRECTLY FROM TO BE ENCRYPTED DATA)======\n");
        printBlocks(cipherTextBlocks, totalBlocks, true);
    }

    // Encryption process starts here
    for (int block = (int)IVpresent; block < totalBlocks; block++)
    {
        // MODE OF OPERATION:
        if (modeOfOperationIndex == 2) // CBC: P[i]=P[i]^C[i-1]
            XORBintoA(cipherTextBlocks[block], cipherTextBlocks[block - 1]);
        else if (modeOfOperationIndex == 3) // PCBC: P[i]=P[i]^C[i-1]^P[i-1]
        {
            XORBintoA(cipherTextBlocks[block], cipherTextBlocks[block - 1]);
            XORBintoA(cipherTextBlocks[block], plainTextBlocks[block - 1]);
        }

        // Pre-round Transformation (or 0th round):
        addRoundKey(cipherTextBlocks[block], roundKeys[0]);

        // 1st, 2nd, ..., (totalRounds - 1)th round
        for (int round = 1; round <= totalRounds - 1; round++)
        {
            subBytes(cipherTextBlocks[block]);
            shiftRows(cipherTextBlocks[block]);
            mixColumns(cipherTextBlocks[block]);
            addRoundKey(cipherTextBlocks[block], roundKeys[round]);
        }

        // totalRounds-th round (no MixColumns)
        subBytes(cipherTextBlocks[block]);
        shiftRows(cipherTextBlocks[block]);
        addRoundKey(cipherTextBlocks[block], roundKeys[totalRounds]);
    }

    // Printing the cipherTextBlocks containing the encrypted text
    if (debugMode)
    {
        printf("\n======INDIVIDUAL ENCRYPTED BLOCKS (AFTER ENCRYPTION)======\n");
        printBlocks(cipherTextBlocks, totalBlocks, false);
    }

    printSeparator();
    printf("  >>>>> FINAL ENCRYPTED DATA <<<<<\n");
    for (int block = 0; block < totalBlocks; block++)
    {
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
                printf("%02X", cipherTextBlocks[block][row][col]);
        }
    }
    printf("\n");
}
