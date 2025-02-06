// Author: anuragmishra-creates (Anurag Mishra)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

const unsigned char sbox[16][16] =
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

const unsigned char inv_Sbox[16][16] =
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

// General purpose: For clearing buffer left after taking string
void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n')
        ;
}

// For KeySchedule: Used for XORing
const unsigned char roundconstants[11] =
    {
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

// Encryption: Left Circular shifting of i'th indexed row i times
void shiftRows(unsigned char array[4][4]) // A 4x4 array is received
{
    int i, j, index;
    char temp[3];

    for (i = 0; i < 4; i++) // This loop takes account of the row being worked upon
    {
        for (j = 0; j < i; j++)
        {
            temp[j] = array[i][j];
        }

        for (j = 0; j < 4 - i; j++)
        {
            array[i][j] = array[i][j + i];
        }

        for (index = 0, j = 4 - i; j < 4; j++, index++)
        {

            array[i][j] = temp[index];
        }
    }
}

// Decryption: Right Circular shifting of i'th indexed row i times
void inv_ShiftRows(unsigned char array[4][4]) // A 4x4 array is received
{
    int i, j, index;
    char temp[3];
    for (i = 0; i < 4; i++) // This loop takes account of the row being worked upon
    {
        for (index = 0, j = 4 - i; j < 4; index++, j++)
        {
            temp[index] = array[i][j];
        }

        for (j = 3 - i; j >= 0; j--)
        {
            array[i][j + i] = array[i][j];
        }

        for (j = 0; j < i; j++)
        {
            array[i][j] = temp[j];
        }
    }
}

// For KeySchedule: Circularly rotates the bytes of the Word to upwards once.
void rotWord(unsigned char word[])
{
    int temp = word[0];
    for (int i = 0; i < 3; i++)
    {
        word[i] = word[i + 1];
    }
    word[3] = temp;
}

// For KeySchedule: Substitutes bytes of the word with sbox
void subWord(unsigned char word[])
{
    for (int i = 0; i < 4; i++)
    { // Hexadecimal digit of the form 0x(XY)
        int Y = word[i] & ((1 << 4) - 1);
        int X = (word[i]) >> 4;

        word[i] = sbox[X][Y];
    }
}

// Encryption: Substitutes the bytes of the word using sbox
void subBytes(unsigned char word[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        { // Hexadecimal digit of the form 0x(XY)
            int Y = word[i][j] & ((1 << 4) - 1);
            int X = word[i][j] >> 4;
            word[i][j] = sbox[X][Y];
        }
    }
}

// Decryption: Substitutes the bytes of the word using inv_Sbox
void inv_SubBytes(unsigned char word[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++) // Hexadecimal digit of the form 0x(XY)
        {
            int Y = word[i][j] & ((1 << 4) - 1);
            int X = word[i][j] >> 4;
            word[i][j] = inv_Sbox[X][Y];
        }
    }
}

// For KeySchedule: XORing of the received word with the round constants
void roundConst(unsigned char word[], int keynum)
{
    word[0] = word[0] ^ roundconstants[keynum];
}

// For KeySchedule: A given round key of index 'keynum' is completed
void finalRoundKey(unsigned char word[], unsigned char round_keys[][4][4], int keynum)
{
    for (int i = 0; i < 4; i++) // Completing first column (word) of round key of index 'keynum'
    {
        round_keys[keynum][i][0] = word[i] ^ round_keys[keynum - 1][i][0];
    }

    for (int j = 1; j < 4; j++) // Completing the second, the third and the fourth columns (words) of round key of index 'keynum'
    {
        for (int i = 0; i < 4; i++)
        {
            round_keys[keynum][i][j] = round_keys[keynum][i][j - 1] ^ round_keys[keynum - 1][i][j];
        }
    }
}

// All 11 rounds keys from the cipher key are generated by this function
void keySchedule(unsigned char round_keys[][4][4], unsigned char ciph_key[4][4])
{
    unsigned char temp_vect[4]; // a 4x1 column vector to be intialized with 4th column (word) of kth round key

    for (int i = 0; i < 4; i++) // Assigning Cipher key to the zeroth key
    {
        for (int j = 0; j < 4; j++)
        {
            round_keys[0][i][j] = ciph_key[i][j];
        }
    }

    for (int k = 1; k < 11; k++) // Generating kth key where k>0
    {
        for (int i = 0; i < 4; i++)
        {
            temp_vect[i] = round_keys[k - 1][i][3]; // Key k>0 is derived from previous round key k-1
        }
        rotWord(temp_vect);
        subWord(temp_vect);
        roundConst(temp_vect, k);
        finalRoundKey(temp_vect, round_keys, k);
    }
}

// For Encryption/Decryption: Used for multiplying two hexadecimal numbers of 1 byte in mixColumns and inv_MixColumns
unsigned char multiply(unsigned char a, unsigned char b)
{
    unsigned short int temp_multiply = 0; // Stores temporary multiplication of a and b in 16 bits

    for (int i = 0; i < 8; i++) // Traverses 8 bits of 'a'
    {
        for (int j = 0; j < 8; j++) // Traverses 8 bits of 'b'
        {
            if (((a & (1 << i)) == (1 << i)) && ((b & (1 << j)) == (1 << j))) // If i'th bit of a and j'th bit of b are on
                temp_multiply = temp_multiply ^ (1 << (i + j));
        }
    }
    unsigned short int irreducible_poly = 0x1B;
    for (int k = 8; k < 15; k++)
    {
        if ((temp_multiply & (1 << k)) == (1 << k)) // If k'th bit of temp_multiply is on
        {
            temp_multiply = temp_multiply ^ irreducible_poly; // Applying irreducible polynomial Theorem
        }
        irreducible_poly = irreducible_poly << 1;
    }
    unsigned char multiply = temp_multiply & 0xFF; // Extracts the last 8 bits since (0xFF) in hexadecimal is (11111111) in binary
    return (multiply);
}

// For Encryption: Multiplies the state (temporary stage of a block being encrypted) with a fixed matrix
void mixColumns(unsigned char state[4][4])
{
    unsigned char matrix[4][4] =
        {
            {0x02, 0x03, 0x01, 0x01},
            {0x01, 0x02, 0x03, 0x01},
            {0x01, 0x01, 0x02, 0x03},
            {0x03, 0x01, 0x01, 0x02}};

    unsigned char result[4][4];

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result[i][j] =
                multiply(matrix[i][0], state[0][j]) ^
                multiply(matrix[i][1], state[1][j]) ^
                multiply(matrix[i][2], state[2][j]) ^
                multiply(matrix[i][3], state[3][j]);
        }
    }
    // Copy result back to state matrix
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            state[i][j] = result[i][j];
        }
    }
}

// For Decryption: Multiplies the state (temporary stage of a block being decrypted) with a fixed matrix
void inv_MixColumns(unsigned char state[4][4])
{
    unsigned char matrix[4][4] =
        {
            {0x0e, 0x0b, 0x0d, 0x09},
            {0x09, 0x0e, 0x0b, 0x0d},
            {0x0d, 0x09, 0x0e, 0x0b},
            {0x0b, 0x0d, 0x09, 0x0e}};

    unsigned char result[4][4];

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result[i][j] =
                multiply(matrix[i][0], state[0][j]) ^
                multiply(matrix[i][1], state[1][j]) ^
                multiply(matrix[i][2], state[2][j]) ^
                multiply(matrix[i][3], state[3][j]);
        }
    }
    // Copy result back to state matrix
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            state[i][j] = result[i][j];
        }
    }
}

// For Encryption/Decryption: XORing the state with roundkeys
void addRoundKey(unsigned char ptext[4][4], unsigned char roundKey[4][4])
{
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            ptext[row][col] ^= roundKey[row][col];
        }
    }
}

// For Encryption: Generates the required number of 4x4 blocks from input string (for encryption)
void blockGenerate(int block, char txt[], unsigned char array[][4][4], int txtsize)
{
    int flag = 0;                          // Checks if we have started padding
    for (int k = 0, c = 0; k < block; k++) // Generating kth block
    {
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (flag == 1)
                {
                    array[k][row][col] = ' ';
                }
                else if (c == txtsize)
                {
                    flag = 1;
                    array[k][row][col] = ' ';
                }
                else
                {
                    array[k][row][col] = txt[c];
                    c++;
                }
            }
        }
    }
}

// For Encryption: Generates encrypted string from individual blocks
char *single_line(unsigned char array[][4][4], int blocks)
{
    char *txt = (char *)malloc(16 * blocks * sizeof(char) + 1);
    int pos = 0;
    for (int k = 0; k < blocks; k++)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                txt[pos++] = array[k][i][j];
        }
    }
    txt[pos] = '\0';
    return txt;
}

// For Decryption: Converts a string of characters to an actual array of hexadecimal numbers
void stringToHex(char txt[], unsigned char hexstr[], int hexstrsize)
{
    unsigned char value; // Value of digit extracted in hexadecimal
    int i = 0, j = 0;
    int digit; // Decides the number of digits left in a single number

    while (i < (2 * hexstrsize))
    {
        digit = 2;
        hexstr[j] = 0;

        while (digit > 0)
        {
            if (txt[i] >= '0' && txt[i] <= '9')
            {
                value = txt[i] - '0';
            }
            else if (txt[i] >= 'a' && txt[i] <= 'f')
            {
                value = txt[i] - 'a' + 10;
            }
            else if (txt[i] >= 'A' && txt[i] <= 'F')
            {
                value = txt[i] - 'A' + 10;
            }

            hexstr[j] = hexstr[j] + value * pow(16, digit - 1);
            digit--;
            i++;
        }
        j++;
    }
    hexstr[j] = '\0';
}

// For decryption : function recieves the encrypted text and applies all inverse functions in right order
//                  to decrypt the text given by the user
void inverse_Cypher(char *txt, unsigned char roundkeys[][4][4])
{

    int size = strlen(txt) / 2; // Size of actual encrypted data to be stored from txt to hexstr

    unsigned char hexstr[size + 1]; // Extra size for NULL character
    stringToHex(txt, hexstr, size);

    int blocks = ((size % 16) ? (size / 16 + 1) : (size / 16));
    unsigned char encrypted_text[blocks][4][4];

    // now fill the 3d array with the encrypted text
    blockGenerate(blocks, hexstr, encrypted_text, size);

    // Printing the blocks containing the encrypted text
    printf("\n--------INDIVIDUAL BLOCKS (DIRECTLY FROM ENCRYPTED DATA)--------\n");
    for (int k = 0; k < blocks; k++)
    {
        printf("\n-----//BLOCK %d//-----\n\n", k + 1);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                printf("0x%02X ", encrypted_text[k][i][j]);
            printf("\n");
        }
        printf("\n");
    }

    // beginning decryption process here

    for (int current_block = 0; current_block < blocks; current_block++)
    {
        // encrypted_text is the 3d matrix having the different blocks
        addRoundKey(encrypted_text[current_block], roundkeys[10]);
        // Main rounds
        for (int round = 9; round > 0; round--)
        {
            inv_ShiftRows(encrypted_text[current_block]);
            inv_SubBytes(encrypted_text[current_block]);
            addRoundKey(encrypted_text[current_block], roundkeys[round]);
            inv_MixColumns(encrypted_text[current_block]);
        }

        // Final round
        inv_ShiftRows(encrypted_text[current_block]);
        inv_SubBytes(encrypted_text[current_block]);
        addRoundKey(encrypted_text[current_block], roundkeys[0]);
    }

    // Printing the blocks containing the decrypted text
    printf("\n--------INDIVIDUAL DECRYPTED BLOCKS (AFTER DECRYPTION)--------\n");
    for (int k = 0; k < blocks; k++)
    {
        printf("\n-----//BLOCK %d//-----\n\n", k + 1);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                printf("%c ", encrypted_text[k][i][j]);
            printf("\n");
        }
        printf("\n");
    }

    printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\n***FINAL DECRYPTED DATA:\n%s\n", single_line(encrypted_text, blocks));
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

// For encryption : function recieves the simple text and applies all encryption functions in right order
//                  to encrypt the text given by the user
void encrypt_cypher(char *txt, unsigned char roundkeys[][4][4])
{
    int size = strlen(txt);
    int blocks = ((size % 16) ? (size / 16 + 1) : (size / 16));
    unsigned char ptext[blocks][4][4];

    // now fill the 3d array with the encrypted text
    blockGenerate(blocks, txt, ptext, size);

    // Printing the blocks containing the plain text
    printf("\n--------INDIVIDUAL BLOCKS (DIRECTLY FROM TO BE ENCRYPTED DATA)--------\n");

    for (int k = 0; k < blocks; k++)
    {
        printf("\n-----//BLOCK %d//-----\n\n", k + 1);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                printf("%c ", ptext[k][i][j]);
            printf("\n");
        }
        printf("\n");
    }

    // Encryption process starts here
    for (int current_block = 0; current_block < blocks; current_block++)
    {
        addRoundKey(ptext[current_block], roundkeys[0]);
        // Main rounds
        for (int round = 1; round < 10; round++)
        {
            subBytes(ptext[current_block]);
            shiftRows(ptext[current_block]);
            mixColumns(ptext[current_block]);
            addRoundKey(ptext[current_block], roundkeys[round]);
        }

        // Final round
        subBytes(ptext[current_block]);
        shiftRows(ptext[current_block]);
        addRoundKey(ptext[current_block], roundkeys[10]);
    }

    // Print the blocks containing the encrypted text
    printf("\n--------INDIVIDUAL ENCRYPTED BLOCKS (AFTER ENCRYPTION)--------\n");

    for (int k = 0; k < blocks; k++)
    {
        printf("\n-----//BLOCK %d//-----\n\n", k + 1);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                printf("0x%02X ", ptext[k][i][j]);
            printf("\n");
        }
        printf("\n");
    }
    printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------");

    printf("\n***FINAL ENCRYPTED DATA (in character format):\n%s\n", single_line(ptext, blocks));
    printf("\n***FINAL ENCRYPTED DATA (in hexadecimal format):\n");
    for (int k = 0; k < blocks; k++)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                printf("%02X", ptext[k][i][j]);
        }
    }
    printf("\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

// For getting string for encryption/decryption :
char *get_string()
{
    char *str = (char *)malloc(40960 * sizeof(char));
    if (str == NULL)
    {
        printf("Not enough memory");
        exit(0);
    }
    char *file_name = (char *)malloc(100 * sizeof(char));

    int option;
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Choose your preference method:\n");
    printf("-> Press '1' to enter string manually on terminal\n-> Press '2' to import string from a text file\n\nYour choice:\t");
    scanf("%d", &option);
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    clearBuffer();
    switch (option)
    {
    case 1:
        printf("\nEnter the data:\n");
        scanf("%[^\n]s", str);
        break;

    case 2:
        printf("Enter name of file ending with .txt:\nFILE NAME->\t");
        scanf("%[^\n]s", file_name);
        FILE *file = fopen(file_name, "r");
        if (file == NULL)
        {
            printf("Error opening file! Not found.\nTry again.\n");
            exit(1);
        }

        if (fgets(str, 40960, file) != NULL)
            printf("\nFollowing string has been read from %s:\n%s\n", file_name, str);
        else
        {
            printf("End of file reached or error occurred.\nTry again.");
            exit(1);
        }
        fclose(file);
        break;

    default:
        printf("Wrong input! Try again.");
    }

    str = realloc(str, (strlen(str) + 1) * sizeof(char));
    free(file_name);
    return str;
}

int main()
{
    unsigned char roundkeys[11][4][4]; // Stores the 11 4x4 round keys
    char cipher_key[] = "abcdefghijklmnop";
    unsigned char cipher_key_final[4][4];

    int c = 0, option;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            cipher_key_final[row][col] = cipher_key[c++];
        }
    }

    keySchedule(roundkeys, cipher_key_final); // Formulation of round keys
    char *str, *txt;

    do
    {
        printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("What do you want to do?\n-> Press '1' for Encryption\n-> Press '2' for Decryption\n-> Press '3' to Exit Program\n\nYour choice:\t");
        scanf("%d", &option);
        printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        switch (option)
        {
        case 1: // ENCRYPTION
            printf("\n****************************************************************  ENCRYPTION  ************************************************************************\n\n");
            str = get_string();

            // CALLING ENCRYPTION FUNCTION
            encrypt_cypher(str, roundkeys);
            free(str);
            break;

        case 2: // DECRYPTION
            printf("\n****************************************************************  DECRYPTION  ************************************************************************\n\n");
            txt = get_string();
            // CALLING DECRYPTION FUNCTION
            inverse_Cypher(txt, roundkeys);
            free(txt);
            break;

        case 3:
            printf("The program has been successfully terminated.\n");
            printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            return 0;

        default:
            printf("Invalid input\n");
        }
    } while (option != 3);

    return 0;
}