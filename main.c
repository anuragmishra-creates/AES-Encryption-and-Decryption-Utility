// Author: anuragmishra-creates (Anurag Mishra)

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "include/AESConstants.h"
#include "include/AESHelpers.h"
#include "include/AESKeySchedule.h"
#include "include/AESEncryption.h"
#include "include/AESDecryption.h"
#include "include/AESGlobals.h"

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

// Unique color for numbers
#define NUM_COLOR CYAN

int main()
{
    uint8_t roundKeys[15][4][4]; // Stores the 15 4x4 round keys
    char cipherKey[33] = "abcdefghijklmnop";
    uint8_t cipherKeyBlock[4][8];
    int modeOfOperationIndex = 1;
    char modeOfOperation[5] = "ECB";

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
        printf(BOLD RED "  AES Encryption & Decryption Utility (by anuragmishra-creates)\n" RESET);
        printf(BOLD YELLOW "  [ AES-%d bit ]\n" RESET, keySizeInBits);
        printf(BOLD YELLOW "  [ Current Cipher Key: %s ]\n" RESET, cipherKey);
        printf(BOLD YELLOW "  [ Current Mode of Operation: %s ]\n" RESET, modeOfOperation);
        printSeparator();
        printf(GREEN "  What do you want to do?\n" RESET);
        printf("  " NUM_COLOR "[1]" RESET GREEN " Encrypt a Message\n" RESET);
        printf("  " NUM_COLOR "[2]" RESET GREEN " Decrypt a Message\n" RESET);
        printf("  " NUM_COLOR "[3]" RESET GREEN " Change the Cipher Key\n" RESET);
        printf("  " NUM_COLOR "[4]" RESET GREEN " Change the Mode of Operation\n" RESET);
        printf("  " NUM_COLOR "[5]" RESET GREEN " %s Debug Mode\n" RESET, debugMode ? "Disable" : "Enable");
        printf("  " NUM_COLOR "[6]" RESET GREEN " Exit Program\n" RESET);
        printf("\n  Your choice: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1: // ENCRYPTION
            printSeparator();
            printf(BOLD RED "  ENCRYPTION mode activated\n" RESET);
            printSeparator();
            str = inputString();

            encryptAES(str, roundKeys, modeOfOperationIndex);
            free(str);
            break;

        case 2: // DECRYPTION
            printSeparator();
            printf(BOLD RED "  DECRYPTION mode activated\n" RESET);
            printSeparator();
            txt = inputString();

            decryptAES(txt, roundKeys, modeOfOperationIndex);
            free(txt);
            break;

        case 3: // CIPHER KEY CHANGE
            printSeparator();
            printf(MAGENTA "  Choose the AES Cipher Key size (in bits):\n" RESET);
            printf("  " NUM_COLOR "[128]" RESET MAGENTA " 128 bit (or 16 bytes)\n" RESET);
            printf("  " NUM_COLOR "[192]" RESET MAGENTA " 192 bit (or 24 bytes)\n" RESET);
            printf("  " NUM_COLOR "[256]" RESET MAGENTA " 256 bit (or 32 bytes)\n" RESET);
            printf("  " NUM_COLOR "[Default]" RESET MAGENTA " Proceed without change in the Cipher Key size\n" RESET);
            printf("\n  Your choice: ");
            scanf("%d", &option2);
            printSeparator();
            if (option2 == 128 || option2 == 192 || option2 == 256)
                keySizeInBits = option2;

            printf(MAGENTA "  Enter the Cipher Key:\n" RESET);
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

        case 4: // MODE OF OPERATION CHANGE
            printSeparator();
            printf(RED "  Choose the AES Mode of Operation:\n" RESET);
            printf("  " NUM_COLOR "[1]" RESET RED " ECB:  Electronic Code Book\n" RESET);
            printf("  " NUM_COLOR "[2]" RESET RED " CBC:  Cipher Block Chaining\n" RESET);
            printf("  " NUM_COLOR "[3]" RESET RED " PCBC: Propagating Cipher Block Chaining\n" RESET);
            printf("  " NUM_COLOR "[Default]" RESET RED " Proceed without change in the Mode of Operation\n");
            printf("\n  Your choice: ");
            scanf("%d", &option2);
            printSeparator();
            if (option2 == 1 || option2 == 2 || option2 == 3)
            {
                modeOfOperationIndex = option2;
                if (modeOfOperationIndex == 1)
                    strcpy(modeOfOperation, "ECB");
                else if (modeOfOperationIndex == 2)
                    strcpy(modeOfOperation, "CBC");
                else if (modeOfOperationIndex == 3)
                    strcpy(modeOfOperation, "PCBC");
            }
            clearBuffer();
            break;

        case 5: // TOGGLE DEBUG MODE
            printSeparator();
            debugMode = !debugMode;
            printf(CYAN "  [ Debugging mode has been %s! ]\n" RESET,
                   debugMode ? "enabled" : "disabled");
            break;

        case 6: // EXIT
            printSeparator();
            printf(BOLD WHITE "  The program has been successfully terminated.\n" RESET);
            printSeparator();
            break;

        default:
            printf(RED "  Invalid input! Try again.\n" RESET);
        }

    } while (option != 6);

    return 0;
}
