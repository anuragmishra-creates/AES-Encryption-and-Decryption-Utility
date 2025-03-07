# Advanced-Encryption-Standard-128-bit

## USAGE CAUTION:
- For decrypting purpose kindly only use the hexadecimal format obtained from encryption. This is because of the limitation of the terminal that caused issue with the newline (\n or ASCII 10) being read as the end of the to be decrypted (encrypted) string.

## Author
- **Anurag Mishra (anuragmishra-creates)**
- **Ayush Gautam (Zman00089)** 

## Motivation
Encryption is crucial in today's digital age, where information security is paramount. AES is one of the widely used encryption algorithms, alongside SHA, RSA, ECC, etc. Our project explores AES encryption due to its significance in securing sensitive data in industries like healthcare, finance, and government. Applications include:
- Secure email and messaging apps.
- Protecting cloud storage data from unauthorized access.
- Enhancing cryptocurrency security through cryptographic puzzles.

## Important Highlights
- Options to enter text manually or import from a `.txt` file.
- Encrypted/decrypted text provided in both string and hex code formats.
- Functions for implementing AES encryption ensure smooth workflow and readability.

## Concepts Explored
- **AES Algorithm**: Encryption (substitution, row shifting, column mixing, round key addition) and decryption (inverse operations).
- **Hexadecimal Format Handling**: Using `unsigned char` data type in C.
- **Data Blocks**: 2D array generation.
- **Bit Manipulation**: Functions like `multiply()`.
- **File Input/Output Handling**.

## Areas of Improvement
- Support for different key sizes (192-bit, 256-bit) beyond the current 128-bit key.
- Complete software with open-source GUI libraries in C.
- Automated copy-paste functionality.
- Support for encrypting other data formats beyond text.
- Algorithm optimization for faster execution.

## Future Scope
- **OCR Integration**: Encrypting scanned publications/research papers for secure storage.
- **Website Security**: Enhancing government site security by encrypting transmitted data.
- **Industry Adoption**: AES remains a NIST-standardized algorithm and will continue to be widely used.

## Functions Used

### Key Schedule Functions
- **subWord**: Substitutes bytes using S-box.
- **roundConst**: XORs word with round constants.
- **keySchedule**: Generates all 11 round keys.
- **finalRoundKey**: Creates round key for a specific index.

### Encryption Functions
- **shiftRows**: Shifts rows left based on index.
- **rotWord**: Rotates word bytes upward.
- **subBytes**: Substitutes bytes using S-box.
- **multiply**: Multiplies two 1-byte hex numbers in `mixColumns` and `inv_MixColumns`.
- **mixColumns**: Multiplies state with a fixed matrix.
- **addRoundKey**: XORs state with round keys.
- **blockGenerate**: Generates required 4x4 blocks.
- **single_line**: Encrypts string from blocks.
- **encrypt_cypher**: Applies encryption functions in order.
- **get_string**: Retrieves string for encryption/decryption.

### Decryption Functions
- **inv_subBytes**: Uses inverse S-box for decryption.
- **inv_ShiftRows**: Shifts rows right based on index.
- **inv_MixColumns**: Multiplies state with a fixed matrix.
- **addRoundKey**: XORs state with round keys.
- **stringToHex**: Converts string to hexadecimal array.
- **inverse_Cypher**: Applies decryption functions in order.
- **get_string**: Retrieves string for encryption/decryption.
- **blockGenerate**: Generates required 4x4 blocks.

## Issues Encountered
### Newline Character in Encrypted Text
- The presence of ASCII 10 (`newline`) caused issues with `gets()`, `scanf()`, `fgets()`, and `fscanf()`.
- Placeholders were not feasible due to full ASCII range usage.
- Unicode placeholders were discarded due to size limitations.
- **Solution**: Representing encrypted text as a hex string (e.g., `0xAB 0x12 0x3E` as `AB123E`).
- **Implementation**: Created `stringToHex` function to store and retrieve encrypted text safely.

---
This project enhances data security through AES encryption and provides a foundation for future cryptographic implementations.
