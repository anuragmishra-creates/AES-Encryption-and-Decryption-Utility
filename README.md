# AES Encryption & Decryption Utility
This is a command-line utility for performing AES (Advanced Encryption Standard) encryption and decryption. The program supports key sizes of 128, 192, and 256 bits, and allows users to input messages manually or from a text file.

## Author
- **Anurag Mishra (anuragmishra-creates)**

## Features
- **AES-128, AES-192, and AES-256 Support**: Choose between three standard cipher key sizes.
- **Key Management**: Easily change the cipher key, which is automatically padded or truncated to the correct length.
- **Flexible Input**: Enter your message directly or read it from a `.txt` file.
- **Debug Mode**: A toggleable debug mode provides detailed output of the encryption and decryption processes, showing the state of the blocks at each step.
- **Cross-Platform Compatibility**  : Works on Linux, macOS, and Windows (no modification).

## How to Compile and Run
To compile and run the program, you can use a standard C compiler like GCC:

```bash
gcc AES.c -o aes_utility -lm
./aes_utility
