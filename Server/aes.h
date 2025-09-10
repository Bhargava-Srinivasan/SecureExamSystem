#ifndef AES_H
#define AES_H

#include <openssl/aes.h>
#include <openssl/rand.h>

#define AES_KEY_SIZE 32  // AES-256 key size in bytes
#define AES_BLOCK_SIZE 16  // AES block size

// Function to generate a random AES key
void generate_aes_key(unsigned char *key, unsigned char *iv);

// AES Encryption function
int aes_encrypt(unsigned char *key, unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext, unsigned char *iv);

// AES Decryption function
int aes_decrypt(unsigned char *key, unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext, unsigned char *iv);

#endif

