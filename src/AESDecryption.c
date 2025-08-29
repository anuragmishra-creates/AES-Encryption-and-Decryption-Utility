#include "../include/AESDecryption.h"
#include "../include/AESConstants.h"
#include "../include/AESHelpers.h"
#include "../include/AESGlobals.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Decryption: Right Circular shifting of row'th indexed row 'row' times
void inverseShiftRows(uint8_t array[4][4]) // A 4x4 array is received
{
    for (int row = 0; row < 4; row++)
    {
        reverse(array[row], 0, 4 - row);
        reverse(array[row], 4 - row, 4);
        reverse(array[row], 0, 4);
    }
}

// Decryption: Substitutes the bytes of the word using inverseSBox
void inverseSubBytes(uint8_t word[4][4])
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++) // Hexadecimal digit of the form 0x(XY)
        {
            int Y = word[row][col] & ((1 << 4) - 1);
            int X = word[row][col] >> 4;
            word[row][col] = inverseSBox[X][Y];
        }
    }
}

// For Decryption: Multiplies the state (temporary stage of a block being decrypted) with a fixed matrix
void inverseMixColumns(uint8_t state[4][4])
{
    uint8_t matrix[4][4] =
        {
            {0x0e, 0x0b, 0x0d, 0x09},
            {0x09, 0x0e, 0x0b, 0x0d},
            {0x0d, 0x09, 0x0e, 0x0b},
            {0x0b, 0x0d, 0x09, 0x0e}};

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

// For decryption : function recieves the encrypted text and applies all inverse functions in right order
//                  to decrypt the text given by the user
void decryptAES(char *txt, uint8_t roundKeys[15][4][4], int modeOfOperationIndex)
{
    bool IVpresent = (modeOfOperationIndex == 2 || modeOfOperationIndex == 3);
    int keySizeInBytes = keySizeInBits / 8;
    int Nk = keySizeInBytes / 4; // Number of words in the cipher key
    int totalRounds = Nk + 6;    // AES standard: Rounds = Nk + 6

    int size = strlen(txt) / 2;
    int totalBlocks = (size + 15) / 16;          // Basically, ceil(size/16.0);
    uint8_t cipherTextBlocks[totalBlocks][4][4]; // Static
    uint8_t plainTextBlocks[totalBlocks][4][4];  // Dynamic: The name refers to the final state it will reach

    uint8_t hexstr[size + 1]; // Extra size for NULL character
    charStringToHexString(txt, hexstr, size);

    // Breaking the text content to blocks each of size 4x4
    blockGenerate(hexstr, size, cipherTextBlocks, totalBlocks, false);
    blockGenerate(hexstr, size, plainTextBlocks, totalBlocks, false);

    // If required in the current mode of operation, storing 0 matrix at plainTextBlocks[0]=0
    if (IVpresent)
    {
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
                plainTextBlocks[0][row][col] = 0;
        }
    }

    // Printing the cipherTextBlocks containing the text before decryption
    if (debugMode)
    {
        printf("\n======INDIVIDUAL BLOCKS (DIRECTLY FROM ENCRYPTED DATA)======\n");
        printBlocks(cipherTextBlocks, totalBlocks, false);
    }

    // Decryption process starts here
    for (int block = (int)IVpresent; block < totalBlocks; block++)
    {
        // Pre-round Transformation (or totalRounds-th round)
        addRoundKey(plainTextBlocks[block], roundKeys[totalRounds]);

        // (totalRounds-1)'th round, ..., 1st round
        for (int round = totalRounds - 1; round >= 1; round--)
        {
            inverseShiftRows(plainTextBlocks[block]);
            inverseSubBytes(plainTextBlocks[block]);
            addRoundKey(plainTextBlocks[block], roundKeys[round]);
            inverseMixColumns(plainTextBlocks[block]);
        }

        // 0th round
        inverseShiftRows(plainTextBlocks[block]);
        inverseSubBytes(plainTextBlocks[block]);
        addRoundKey(plainTextBlocks[block], roundKeys[0]);

        // MODE OF OPERATION:
        if (modeOfOperationIndex == 2) // CBC: P[i]=P[i]^C[i-1]
            XORBintoA(plainTextBlocks[block], cipherTextBlocks[block - 1]);
        else if (modeOfOperationIndex == 3) // PCBC: P[i]=P[i]^C[i-1]^P[i-1]
        {
            XORBintoA(plainTextBlocks[block], cipherTextBlocks[block - 1]);
            XORBintoA(plainTextBlocks[block], plainTextBlocks[block - 1]);
        }
    }

    // Printing the plainTextBlocks containing the decrypted text
    if (debugMode)
    {
        printf("\n======INDIVIDUAL DECRYPTED BLOCKS (AFTER DECRYPTION)======\n");
        printBlocks(plainTextBlocks, totalBlocks, true);
    }

    printSeparator();
    printf("  >>>>> FINAL DECRYPTED DATA <<<<<\n%s\n", lineGenerate(plainTextBlocks, totalBlocks, IVpresent));
}
