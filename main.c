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
        printf("  AES Encryption & Decryption Utility (by anuragmishra-creates)\n");
        printf("  [ AES-%d bit ]\n", keySizeInBits);
        printf("  [ Current Cipher Key: %s ]\n", cipherKey);
        printf("  [ Current Mode of Operation: %s ]\n", modeOfOperation);
        printSeparator();
        printf("  What do you want to do?\n");
        printf("  [1] Encrypt a Message\n");
        printf("  [2] Decrypt a Message\n");
        printf("  [3] Change the Cipher Key\n");
        printf("  [4] Change the Mode of Operation\n");
        printf("  [5] %s Debug Mode\n", debugMode ? "Disable" : "Enable");
        printf("  [6] Exit Program\n");
        printf("\n  Your choice: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1: // ENCRYPTION
            printSeparator();
            printf("  ENCRYPTION mode activated\n");
            printSeparator();
            str = inputString();

            encryptAES(str, roundKeys, modeOfOperationIndex);
            free(str);
            break;

        case 2: // DECRYPTION
            printSeparator();
            printf("  DECRYPTION mode activated\n");
            printSeparator();
            txt = inputString();

            decryptAES(txt, roundKeys, modeOfOperationIndex);
            free(txt);
            break;

        case 3: // CIPHER KEY CHANGE
            printSeparator();
            printf("  Choose the AES Cipher Key size (in bits):\n");
            printf("  [128] 128 bit (or 16 bytes)\n");
            printf("  [192] 192 bit (or 24 bytes)\n");
            printf("  [256] 256 bit (or 32 bytes)\n");
            printf("  [Default] Proceed without change in the Cipher Key size\n");
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

        case 4: // MODE OF OPERATION CHANGE
            printSeparator();
            printf("  Choose the AES Mode of Operation:\n");
            printf("  [1] ECB:  Electronic Code Book\n");
            printf("  [2] CBC:  Cipher Block Chaining\n");
            printf("  [3] PCBC: Propagating Cipher Block Chaining\n");
            printf("  [Default] Proceed without change in the Mode of Operation\n");
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
            printf("  [ Debugging mode has been %s! ]", debugMode ? "enabled" : "disabled");
            break;

        case 6: // EXIT
            printSeparator();
            printf("  The program has been successfully terminated.\n");
            printSeparator();
            break;

        default:
            printf("  Invalid input! Try again.\n");
        }

    } while (option != 6);

    return 0;
}