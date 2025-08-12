// Author: anuragmishra-creates (Anurag Mishra)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

int keySizeInBits = 128;
bool debugMode = false;

void printSeparator()
{
    printf("==========================================================================================================\n");
}

const uint8_t sbox[16][16] =
    //     0     1     2    3      4     5     6     7     8     9     a     b     c    d      e     f
    {
        {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76}, // 0
        {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0}, // 1
        {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15}, // 2
        {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75}, // 3
        {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84}, // 4
        {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf}, // 5
        {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8}, // 6
        {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2}, // 7
        {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73}, // 8
        {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb}, // 9
        {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79}, // a
        {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08}, // b
        {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a}, // c
        {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e}, // d
        {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf}, // e
        {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}  // f
};

const uint8_t inverseSBox[16][16] =
    //     0     1     2    3      4     5     6     7     8     9     a     b     c    d      e     f
    {
        {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb}, // 0
        {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb}, // 1
        {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e}, // 2
        {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25}, // 3
        {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92}, // 4
        {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84}, // 5
        {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06}, // 6
        {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b}, // 7
        {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73}, // 8
        {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e}, // 9
        {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b}, // a
        {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4}, // b
        {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f}, // c
        {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef}, // d
        {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61}, // e
        {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}  // f
};

// To discard the newline (and any stray characters) left in the input buffer after scanf() so that the next input isn’t skipped.
// Used after scanf() reads something that doesn’t consume the newline.
void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        /* Empty */;
}

// For KeySchedule: Used for XORing
const uint8_t roundConstants[11] =
    {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

void reverse(uint8_t array[], int begin, int end)
{
    if (begin >= end)
        return;

    int i = begin, j = end - 1;
    while (i < j)
    {
        uint8_t temp = array[i];
        array[i] = array[j];
        array[j] = temp;
        i++, j--;
    }
}

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

// For KeySchedule: Circularly rotates the bytes of the Word to upwards once.
uint32_t rotWord(uint32_t word)
{
    // word = [0th byte - 1st byte - 2nd byte - 3rd byte]
    uint8_t byte0 = (word >> 24) & 0xFF;
    word = word << 8;
    word = word | byte0;
    return word;
}

// For KeySchedule: Substitutes bytes of the word with sbox
uint32_t subWord(uint32_t word)
{
    // word = [0th byte - 1st byte - 2nd byte - 3rd byte]
    uint32_t result = 0;
    for (int i = 0; i < 4; i++)
    {
        // Extract one byte (from most significant byte to least)
        uint8_t currByte = (word >> (24 - 8 * i)) & 0xFF;

        // Hexadecimal digit of the form 0x(XY)
        uint8_t X = currByte >> 4;
        uint8_t Y = currByte & 0x0F;

        uint8_t substitutedByte = sbox[X][Y];

        // Place the substituted byte back into the result
        result |= (substitutedByte << (24 - 8 * i));
    }
    return result;
}

// For KeySchedule: XORing of the received word with the round constants
uint32_t rCon(uint32_t word, int round)
{
    // word = [0th byte - 1st byte - 2nd byte - 3rd byte]
    word = word ^ (roundConstants[round] << 24);
    return word;
}

// For KeySchedule: Sets the bytes to correct location for each word
uint32_t setByte(int byte0, int byte1, int byte2, int byte3)
{
    // val= [byte0 : byte1 : byte2 : byte3]
    uint32_t val = 0;
    val = val | (byte0 << 24);
    val = val | (byte1 << 16);
    val = val | (byte2 << 8);
    val = val | (byte3 << 0);

    return val;
}

// All 11/13/15 rounds keys from the cipher key are generated by this function
void keySchedule(uint8_t roundKeys[15][4][4], uint8_t cipherKey[4][8])
{
    if (keySizeInBits != 128 && keySizeInBits != 192 && keySizeInBits != 256)
    {
        printf("  Error! Invalid key size!");
        exit(EXIT_FAILURE);
    }
    int keySizeInBytes = keySizeInBits / 8;
    int Nk = keySizeInBytes / 4;            // Number of words in the cipher key
    int totalRounds = Nk + 6;               // AES standard: Rounds = Nk + 6
    int totalWords = (totalRounds + 1) * 4; // Total words for all round keys

    uint32_t words[totalWords]; // Each word = 4 bytes [0th - 1st - 2nd - 3rd]

    // Generating i'th word
    for (int i = 0; i < totalWords; i++)
    {
        if (i < Nk) // Step 1: Directly from cipher key
            words[i] = setByte(cipherKey[0][i], cipherKey[1][i], cipherKey[2][i], cipherKey[3][i]);
        else if (i % Nk == 0) // Step 2a: RotWord + SubWord + Rcon
            words[i] = words[i - Nk] ^ rCon(subWord(rotWord(words[i - 1])), i / Nk);
        else if (Nk == 8 && (i % Nk == 4)) // Step 2b: Special case for AES-256
            words[i] = words[i - Nk] ^ subWord(words[i - 1]);
        else // Step 2c: Default case
            words[i] = words[i - Nk] ^ words[i - 1];
    }

    // Load words into round keys
    for (int round = 0; round <= totalRounds; round++)
    {
        for (int col = 0; col < 4; col++)
        {
            uint32_t word = words[round * 4 + col];
            roundKeys[round][0][col] = (word >> 24) & 0xFF;
            roundKeys[round][1][col] = (word >> 16) & 0xFF;
            roundKeys[round][2][col] = (word >> 8) & 0xFF;
            roundKeys[round][3][col] = word & 0xFF;
        }
    }
}

// For Encryption/Decryption: Used for multiplying two hexadecimal numbers of 1 byte in mixColumns and inverseMixColumns
uint8_t multiply(uint8_t a, uint8_t b)
{
    unsigned short int tempMultiplication = 0; // Stores temporary multiplication of a and b in 16 bits

    for (int i = 0; i < 8; i++) // Traverses 8 bits of 'a'
    {
        for (int j = 0; j < 8; j++) // Traverses 8 bits of 'b'
        {
            if (((a & (1 << i)) == (1 << i)) && ((b & (1 << j)) == (1 << j))) // If i'th bit of a and j'th bit of b are on
                tempMultiplication = tempMultiplication ^ (1 << (i + j));
        }
    }
    unsigned short int irreduciblePolynomial = 0x1B;
    for (int k = 8; k < 15; k++)
    {
        if ((tempMultiplication & (1 << k)) == (1 << k)) // If k'th bit of tempMultiplication is on
        {
            tempMultiplication = tempMultiplication ^ irreduciblePolynomial; // Applying irreducible polynomial Theorem
        }
        irreduciblePolynomial = irreduciblePolynomial << 1;
    }
    uint8_t multiply = tempMultiplication & 0xFF; // Extracts the last 8 bits since (0xFF) in hexadecimal is (11111111) in binary
    return (multiply);
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

// For Encryption/Decryption: XORing the state with the current roundKey
void addRoundKey(uint8_t stateMatrices[4][4], uint8_t roundKey[4][4])
{
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            stateMatrices[row][col] ^= roundKey[row][col];
        }
    }
}

// For Encryption/Decryption: Generates the required number of 4x4 blocks from input string
void blockGenerate(char txt[], int txtsize, uint8_t stateMatrices[][4][4], int totalStateMatrices)
{
    int c = 0; // Index in txt
    for (int block = 0; block < totalStateMatrices; block++)
    {
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (c < txtsize)
                    stateMatrices[block][row][col] = txt[c++];
                else
                    stateMatrices[block][row][col] = ' '; // Pad with space after txt ends
            }
        }
    }
}

// For Decryption: Serializes the 4x4 blocks into a single string
char *lineGenerate(uint8_t stateMatrices[][4][4], int totalStateMatrices)
{
    char *txt = (char *)malloc(totalStateMatrices * (16 * sizeof(char)) + 1);
    int c = 0; // Index in txt
    for (int block = 0; block < totalStateMatrices; block++)
    {
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
                txt[c++] = stateMatrices[block][row][col];
        }
    }
    txt[c] = '\0';
    return txt;
}

// Helper: Converts a single hex character to its numerical value (0-15)
int hexCharToValue(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    return -1; // Invalid hex character
}

// For Decryption: Converts a hex string (like "AB1F2C") to string: hexstr[] = {0xAB, 0x1F, 0x2C, '\0'}
void charStringToHexString(char txt[], uint8_t hexstr[], int hexstrsize)
{
    for (int i = 0, j = 0; j < hexstrsize; j++)
    {
        int high = hexCharToValue(txt[i++]);
        int low = hexCharToValue(txt[i++]);
        if (high == -1 || low == -1)
        {
            printf("  Found an invalid character. Invalid encrypted string!\n");
            printf("  Terminating the program!");
            exit(EXIT_FAILURE);
        }
        hexstr[j] = (high << 4) | low;
    }
    hexstr[hexstrsize] = '\0';
}

// Prints the individual blocks obtained before/after encryption/decryption
void printBlocks(uint8_t stateMatrices[][4][4], int totalStateMatrices, bool charFormat)
{
    for (int block = 0; block < totalStateMatrices; block++)
    {
        printf("\n====== BLOCK %d ======\n", block);
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (charFormat)
                    printf("%c ", stateMatrices[block][row][col]);
                else // (hexFormat)
                    printf("0x%02X ", stateMatrices[block][row][col]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

// For decryption : function recieves the encrypted text and applies all inverse functions in right order
//                  to decrypt the text given by the user
void decryptAES(char *txt, uint8_t roundKeys[15][4][4])
{
    int keySizeInBytes = keySizeInBits / 8;
    int Nk = keySizeInBytes / 4; // Number of words in the cipher key
    int totalRounds = Nk + 6;    // AES standard: Rounds = Nk + 6

    int size = strlen(txt) / 2;
    int totalStateMatrices = (size + 15) / 16; // Basically, ceil(size/16.0);
    uint8_t stateMatrices[totalStateMatrices][4][4];

    uint8_t hexstr[size + 1]; // Extra size for NULL character
    charStringToHexString(txt, hexstr, size);

    // Breaking the text content to blocks each of size 4x4
    blockGenerate(hexstr, size, stateMatrices, totalStateMatrices);

    // Printing the stateMatrices containing the text before decryption
    if (debugMode)
    {
        printf("\n======INDIVIDUAL BLOCKS (DIRECTLY FROM ENCRYPTED DATA)======\n");
        printBlocks(stateMatrices, totalStateMatrices, false);
    }

    // Decryption process starts here
    for (int block = 0; block < totalStateMatrices; block++)
    {
        // Pre-round Transformation (or totalRounds-th round)
        addRoundKey(stateMatrices[block], roundKeys[totalRounds]);

        // (totalRounds-1)'th round, ..., 1st round
        for (int round = totalRounds - 1; round >= 1; round--)
        {
            inverseShiftRows(stateMatrices[block]);
            inverseSubBytes(stateMatrices[block]);
            addRoundKey(stateMatrices[block], roundKeys[round]);
            inverseMixColumns(stateMatrices[block]);
        }

        // 0th round
        inverseShiftRows(stateMatrices[block]);
        inverseSubBytes(stateMatrices[block]);
        addRoundKey(stateMatrices[block], roundKeys[0]);
    }

    // Printing the stateMatrices containing the decrypted text
    if (debugMode)
    {
        printf("\n======INDIVIDUAL DECRYPTED BLOCKS (AFTER DECRYPTION)======\n");
        printBlocks(stateMatrices, totalStateMatrices, true);
    }

    printSeparator();
    printf("  >>>>> FINAL DECRYPTED DATA <<<<<\n%s\n", lineGenerate(stateMatrices, totalStateMatrices));
}

// For Encryption : The function recieves the text and applies all encryption functions in
//                  the right order to encrypt the text given by the user
void encryptAES(char *txt, uint8_t roundKeys[15][4][4])
{
    int keySizeInBytes = keySizeInBits / 8;
    int Nk = keySizeInBytes / 4; // Number of words in the cipher key
    int totalRounds = Nk + 6;    // AES standard: Rounds = Nk + 6

    int size = strlen(txt);
    int totalStateMatrices = (size + 15) / 16; // Basically, ceil(size/16.0);
    uint8_t stateMatrices[totalStateMatrices][4][4];

    // Breaking the text content to blocks each of size 4x4
    blockGenerate(txt, size, stateMatrices, totalStateMatrices);

    // Printing the stateMatrices containing the text before encryption
    if (debugMode)
    {
        printf("\n======INDIVIDUAL BLOCKS (DIRECTLY FROM TO BE ENCRYPTED DATA)======\n");
        printBlocks(stateMatrices, totalStateMatrices, true);
    }

    // Encryption process starts here
    for (int block = 0; block < totalStateMatrices; block++)
    {
        // Pre-round Transformation (or 0th round):
        addRoundKey(stateMatrices[block], roundKeys[0]);

        // 1st, 2nd, ..., (totalRounds - 1)th round
        for (int round = 1; round <= totalRounds - 1; round++)
        {
            subBytes(stateMatrices[block]);
            shiftRows(stateMatrices[block]);
            mixColumns(stateMatrices[block]);
            addRoundKey(stateMatrices[block], roundKeys[round]);
        }

        // totalRounds-th round (no MixColumns)
        subBytes(stateMatrices[block]);
        shiftRows(stateMatrices[block]);
        addRoundKey(stateMatrices[block], roundKeys[totalRounds]);
    }

    // Printing the stateMatrices containing the encrypted text
    if (debugMode)
    {
        printf("\n======INDIVIDUAL ENCRYPTED BLOCKS (AFTER ENCRYPTION)======\n");
        printBlocks(stateMatrices, totalStateMatrices, false);
    }

    printSeparator();
    printf("  >>>>> FINAL ENCRYPTED DATA <<<<<\n");
    for (int block = 0; block < totalStateMatrices; block++)
    {
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
                printf("%02X", stateMatrices[block][row][col]);
        }
    }
    printf("\n");
}

// For getting string for encryption/decryption :
char *inputString()
{
    // Allocating the memory for the input:
    // Input string can be of size atmost 65,536 characters (64 KBs)
    int size = 65536 * sizeof(char);
    char *str = NULL;
    char fileName[256];
    while (size > 0)
    {
        str = malloc(size);
        if (str != NULL)
        {
            printf("  [ Memory of size %d bytes allocated successfully ]\n", size);
            break;
        }
        size /= 2;
    }
    if (str == NULL)
    {
        printf("  Error! Could NOT allocate any memory for the input!\n");
        printf("  Terminating the program!");
        exit(EXIT_FAILURE);
    }

    // Taking the input:
    int option;
    // Keep on asking for the preferred method till not valid input received
    do
    {
        printSeparator();
        printf("Choose your preference method:\n");
        printf("  [1] Enter the message manually\n");
        printf("  [2] Import the message from a .txt file\n");
        printf("\n  Your choice: ");
        scanf("%d", &option);
        printSeparator();

        clearBuffer();
        FILE *filePtr = NULL;
        switch (option)
        {
        case 1:
            printf("  Enter the message:\n");
            scanf("%[^\n]s", str);
            break;

        case 2:

            // Keep on asking for the correct file name till not valid
            while (filePtr == NULL)
            {
                printf("  Enter name of the file ending with .txt:\nFILE NAME->\t");
                scanf("%[^\n]s", fileName);
                filePtr = fopen(fileName, "r");
                if (filePtr == NULL)
                    printf("  The file %s was NOT found! Try again.\n", fileName);
            }

            // If some issue while reading from the file, then exit immediately
            if (fgets(str, size, filePtr) != NULL)
                printf("  The following message has been read from the file %s:\n%s\n", fileName, str);
            else
            {
                if (feof(filePtr))
                    printf("  End of File reached! Nothing to read from the file %s!\n", fileName);
                else if (ferror(filePtr))
                    printf("  An error occurred while reading from the file %s!\n", fileName);
                else
                    printf("  Unknown error occurred while reading from the file %s!\n", fileName);
                printf("  Terminating the program!");
                exit(EXIT_FAILURE);
            }
            fclose(filePtr);
            break;

        default:
            printf("  Invalid input! Try again.\n");
        }
    } while (option != 1 && option != 2);

    char *tempPtr = realloc(str, (strlen(str) + 1) * sizeof(char));
    if (tempPtr) // Only replace if realloc succeeded
        str = tempPtr;
    return str;
}

int main()
{
    uint8_t roundKeys[15][4][4]; // Stores the 15 4x4 round keys
    char cipherKey[33] = "abcdefghijklmnop";
    uint8_t cipherKeyBlock[4][8];

    for (int row = 0, c = 0; row < 4; row++)
    {
        for (int col = 0; col < (keySizeInBits / 32); col++)
            cipherKeyBlock[row][col] = cipherKey[c++];
    }

    keySchedule(roundKeys, cipherKeyBlock); // Formulation of the round keys

    int option, option2;
    char *str, *txt;
    do
    {
        // Printing the Main Menu:
        printf("\n");
        printSeparator();
        printf("  AES Encryption & Decryption Utility (by anuragmishra-creates)\n");
        printf("  [ AES-%d bit ]\n", keySizeInBits);
        printf("  [ Current Cipher Key: %s ]\n", cipherKey);
        printSeparator();
        printf("  What do you want to do?\n");
        printf("  [1] Encrypt a Message\n");
        printf("  [2] Decrypt a Message\n");
        printf("  [3] Change the Cipher Key\n");
        printf("  [4] %s Debug Mode\n", debugMode ? "Disable" : "Enable");
        printf("  [5] Exit Program\n");
        printf("\n  Your choice: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1: // ENCRYPTION
            printSeparator();
            printf("  ENCRYPTION mode activated\n");
            printSeparator();
            str = inputString();

            encryptAES(str, roundKeys);
            free(str);
            break;

        case 2: // DECRYPTION
            printSeparator();
            printf("  DECRYPTION mode activated\n");
            printSeparator();
            txt = inputString();

            decryptAES(txt, roundKeys);
            free(txt);
            break;

        case 3: // CIPHER KEY CHANGE
            printSeparator();
            printf("  Choose the AES Cipher Key size (in bits):\n");
            printf("  [128] 128 bit (or 16 bytes)\n");
            printf("  [192] 192 bit (or 24 bytes)\n");
            printf("  [256] 256 bit (or 32 bytes)\n");
            printf("  [Default] Proceed without change in the Cipher Key size:\n");
            printf("\n  Your choice: ");
            scanf("%d", &option2);
            printSeparator();
            if (option2 == 128 || option2 == 192 || option2 == 256)
                keySizeInBits = option2;

            printf("  Enter the Cipher Key:\n");
            printf("  (Note: The key will be truncated if too long, or padded with '_' if too short)\n");
            clearBuffer();
            /*
                Note: For fgets(str,5,stdin) stops if:
                (1) Newline is read first, or
                (2) Limit of 4 characters reached, or
                (3) End of File (here stdin)
                Example:
                (1) stdin: "ab\n"
                str: {'a','b','\n','\0'}
                (2) stdin: "abcdefg"
                str: {'a','b','c','d','\0'}
            */
            fgets(cipherKey, (keySizeInBits / 8) + 1, stdin); // Deals with too long. Also, +1 for '\0'
            clearBuffer();                                    // Remove left over elements if too long in the stdin
            int i = strcspn(cipherKey, "\n");                 // In case it is too short
            while (i < keySizeInBits / 8)
                cipherKey[i++] = '_';
            cipherKey[keySizeInBits / 8] = '\0';
            for (int row = 0, c = 0; row < 4; row++)
            {
                for (int col = 0; col < (keySizeInBits / 32); col++)
                    cipherKeyBlock[row][col] = cipherKey[c++];
            }
            keySchedule(roundKeys, cipherKeyBlock);
            break;

        case 4: // TOGGLE DEBUG MODE
            printSeparator();
            debugMode = !debugMode;
            printf("  [ Debugging mode has been %s! ]", debugMode ? "enabled" : "disabled");
            break;

        case 5: // EXIT
            printSeparator();
            printf("  The program has been successfully terminated.\n");
            printSeparator();
            break;

        default:
            printf("  Invalid input! Try again.\n");
        }

    } while (option != 5);

    return 0;
}