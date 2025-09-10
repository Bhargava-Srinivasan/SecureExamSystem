#include "encryption.h"
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>

// Load the RSA private key using EVP_PKEY
EVP_PKEY *load_private_key() {
    FILE *fp = fopen("private.pem", "r");
    if (!fp) {
        perror("Error opening private key file");
        return NULL;
    }

    EVP_PKEY *key = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);

    if (!key) {
        fprintf(stderr, "Error reading private key\n");
        ERR_print_errors_fp(stderr);
    }

    return key;
}

// Load the RSA public key using EVP_PKEY
EVP_PKEY *load_public_key(const char *public_key_path) {
    FILE *fp = fopen("public.pem", "r");
    if (!fp) {
        perror("Unable to open public key file");
        return NULL;
    }

    EVP_PKEY *pkey = PEM_read_PUBKEY(fp, NULL, NULL, NULL);
    fclose(fp);
    return pkey;
}


int rsa_encrypt_with_public_key(const char *public_key_path, const unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext) {
    EVP_PKEY *pkey = load_public_key(public_key_path);
    if (!pkey) return -1;

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx) {
        EVP_PKEY_free(pkey);
        return -1;
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0 ||
        EVP_PKEY_encrypt(ctx, ciphertext, (size_t *)&plaintext_len, plaintext, plaintext_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        return -1;
    }

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pkey);
    return plaintext_len;
}

// Encrypt data using RSA public key (EVP_PKEY)
int rsa_encrypt(EVP_PKEY *pkey, const unsigned char *input, int input_len, unsigned char *output) {
    if (!pkey || !input || !output) {
        fprintf(stderr, "Invalid parameters for rsa_encrypt()\n");
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx) {
        fprintf(stderr, "Error initializing encryption context\n");
        return -1;
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        fprintf(stderr, "EVP_PKEY_encrypt_init failed\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        fprintf(stderr, "Error setting RSA padding\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    size_t output_len;
    if (EVP_PKEY_encrypt(ctx, NULL, &output_len, input, input_len) <= 0) {
        fprintf(stderr, "Error determining encrypted length\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_encrypt(ctx, output, &output_len, input, input_len) <= 0) {
        fprintf(stderr, "Error encrypting data\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    EVP_PKEY_CTX_free(ctx);
    return output_len;
}

// Decrypt data using RSA private key (EVP_PKEY)
int rsa_decrypt(EVP_PKEY *pkey, const unsigned char *input, int input_len, unsigned char *output) {
    if (!pkey || !input || !output) {
        fprintf(stderr, "Invalid parameters for rsa_decrypt()\n");
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx) {
        fprintf(stderr, "Error initializing decryption context\n");
        return -1;
    }

    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        fprintf(stderr, "EVP_PKEY_decrypt_init failed\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        fprintf(stderr, "Error setting RSA padding\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    size_t output_len;
    if (EVP_PKEY_decrypt(ctx, NULL, &output_len, input, input_len) <= 0) {
        fprintf(stderr, "Error determining decrypted length\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    if (EVP_PKEY_decrypt(ctx, output, &output_len, input, input_len) <= 0) {
        fprintf(stderr, "Error decrypting data\n");
        EVP_PKEY_CTX_free(ctx);
        return -1;
    }

    EVP_PKEY_CTX_free(ctx);
    return output_len;
}

