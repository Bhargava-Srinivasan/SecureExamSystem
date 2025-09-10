#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "aes.h"

// RSA Key Loading Functions
EVP_PKEY *load_private_key();
EVP_PKEY *load_public_key();

int rsa_encrypt_with_public_key(const char *public_key_path, const unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext);


// RSA Encryption & Decryption
int rsa_encrypt(EVP_PKEY *pkey, const unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext);
int rsa_decrypt(EVP_PKEY *private_key, const unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext);

// AES Encryption & Decryption
int aes_encrypt(unsigned char *key, unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext, unsigned char *iv);
int aes_decrypt(unsigned char *key, unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext, unsigned char *iv);

#endif

