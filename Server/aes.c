#include "aes.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>

// AES Encryption function
int aes_encrypt(unsigned char *key, unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext, unsigned char *iv) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    // Generate a random IV
    RAND_bytes(iv, AES_BLOCK_SIZE);

    // Create and initialize the context
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    // Encrypt the data
    if (!EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len = len;

    if (!EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

// AES Decryption function
int aes_decrypt(unsigned char *key, unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext, unsigned char *iv) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    // Create and initialize the context
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    // Decrypt the data
    if (!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len = len;

    if (!EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

