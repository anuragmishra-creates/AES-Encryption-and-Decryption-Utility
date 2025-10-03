#include "../include/AESHelpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI Escape Codes for Colors
#define RESET "\033[0m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define BLUE "\033[94m"
#define MAGENTA "\033[95m"
#define CYAN "\033[96m"
#define WHITE "\033[97m"
#define BOLD "\033[1m"

void printSeparator()
{
    printf(BOLD WHITE "====================================================================================================================\n" RESET);
}

// To discard the newline (and any stray characters) left in the input buffer after scanf() so that the next input isn’t skipped.
// Used after scanf() reads something that doesn’t consume the newline.
void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        /* Empty */;
}

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

// For Encryption/Decryption: Generates the required number of 4x4 blocks from input string
void blockGenerate(char txt[], int txtsize, uint8_t stateMatrices[][4][4], int totalBlocks, bool leaveBlock0)
{
    int c = 0; // Index in txt
    for (int block = (int)leaveBlock0; block < totalBlocks; block++)
    {
        for (int i = 0; i < 16; i++)
        {
            stateMatrices[block][i / 4][i % 4] = (c < txtsize) ? txt[c++] : ' ';
        }
    }
}

// For Decryption: Serializes the 4x4 blocks into a single string
char *lineGenerate(uint8_t stateMatrices[][4][4], int totalBlocks, bool IVpresent)
{
    char *txt = (char *)malloc(totalBlocks * (16 * sizeof(char)) + 1);
    int c = 0; // Index in txt
    for (int block = (int)IVpresent; block < totalBlocks; block++)
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
void printBlocks(uint8_t stateMatrices[][4][4], int totalBlocks, bool charFormat)
{
    for (int block = 0; block < totalBlocks; block++)
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

void XORBintoA(uint8_t A[4][4], uint8_t B[4][4])
{
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            A[row][col] ^= B[row][col];
        }
    }
}

// For Encryption/Decryption: XORing the state with the current roundKey
void addRoundKey(uint8_t stateMatrices[4][4], uint8_t roundKey[4][4])
{
    XORBintoA(stateMatrices, roundKey);
}

// For getting string for encryption/decryption :
char *inputString()
{
    int capacity = 65536; // initial capacity (64 KB)
    char *str = malloc(capacity);
    if (!str)
    {
        fprintf(stderr, "Error! Could NOT allocate memory!\n");
        exit(EXIT_FAILURE);
    }
    str[0] = '\0'; // start with empty string

    int option;
    char fileName[256];
    FILE *filePtr = NULL;

    // Ask input method
    do
    {
        printf(BOLD MAGENTA "Choose your preferred method:\n" RESET);
        printf("  " CYAN "[1]" RESET MAGENTA " Enter the message manually (Ctrl+D to end)\n" RESET);
        printf("  " CYAN "[2]" RESET MAGENTA " Import the message from a .txt file\n" RESET);
        printf("\n  Your choice: ");
        scanf("%d", &option);
        printSeparator();
        clearBuffer();

    } while (option != 1 && option != 2);

    // Select input source
    if (option == 1)
    {
        printf(MAGENTA "  Enter your message (press Ctrl+D to finish):\n" RESET);
        filePtr = stdin; // read from terminal
    }
    else
    {
        while (filePtr == NULL)
        {
            printf(MAGENTA "  Enter name of the file ending with .txt:\n" RESET);
            printf(MAGENTA "FILE NAME:  " RESET);
            scanf("%[^\n]", fileName);
            clearBuffer();
            filePtr = fopen(fileName, "r");
            if (!filePtr)
                printf(MAGENTA "  The file %s was NOT found! Try again.\n" RESET, fileName);
        }
    }

    // Read until EOF
    size_t len = 0;
    char buffer[1024]; // temporary chunk
    while (fgets(buffer, sizeof(buffer), filePtr))
    {
        size_t chunkLen = strlen(buffer);
        if (len + chunkLen + 1 > capacity)
        {
            // grow buffer
            capacity *= 2;
            char *tmp = realloc(str, capacity);
            if (!tmp)
            {
                fprintf(stderr, "Error! Could not reallocate memory!\n");
                free(str);
                exit(EXIT_FAILURE);
            }
            str = tmp;
        }
        strcpy(str + len, buffer);
        len += chunkLen;
    }

    // Clear EOF flag so next scanf works fine
    if (filePtr == stdin)
        clearerr(stdin);
    else
        fclose(filePtr);

    return str; // contains everything till EOF or Ctrl+D
}

/* char *inputString()
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
} */