#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include "encryption.h"
#include "database.h"
#include "aes.h"

#define PORT 8080
#define BUFFER_SIZE 4096

// Generate a random AES key and IV
void generate_aes_key(unsigned char *aes_key, unsigned char *iv) {
    RAND_bytes(aes_key, AES_KEY_SIZE);
    RAND_bytes(iv, AES_BLOCK_SIZE);
}

// Function to handle client authentication and exam process
void *client_handler(void *client_socket_ptr) {
    int client_socket = *(int *)client_socket_ptr;
    free(client_socket_ptr);

    printf("New client connected.\n");

    // Load Private Key
    EVP_PKEY *private_key = load_private_key();
    if (!private_key) {
        perror("Failed to load private key");
        close(client_socket);
        return NULL;
    }

    // Receive Encrypted Credentials
    unsigned char encrypted_credentials[256];
    recv(client_socket, encrypted_credentials, sizeof(encrypted_credentials), 0);
    printf("Received encrypted credentials from client.\n");

    // Decrypt Credentials
    unsigned char decrypted_credentials[256];
    int decrypted_len = rsa_decrypt(private_key, encrypted_credentials, 256, decrypted_credentials);
    EVP_PKEY_free(private_key);

    if (decrypted_len <= 0) {
        send(client_socket, "AUTH_FAIL", 9, 0);
        close(client_socket);
        return NULL;
    }

    decrypted_credentials[decrypted_len] = '\0';
    char username[128], password[128];
    sscanf((char *)decrypted_credentials, "%s %s", username, password);

    printf("Username received: %s\n", username);
    printf("Password received: %s\n", password);

    // Authenticate User
    char role[32];  // Added role variable
    if (!authenticate_user(username, password, role)) {
        send(client_socket, "AUTH_FAIL", 9, 0);
        close(client_socket);
        return NULL;
    }
    send(client_socket, "AUTH_SUCCESS", 12, 0);
    printf("User authenticated successfully!\n");

    // Generate AES Session Key and IV
    unsigned char aes_key[AES_KEY_SIZE];
    unsigned char iv[AES_BLOCK_SIZE];  // Added IV
    generate_aes_key(aes_key, iv);

    // Encrypt AES Key with Client's RSA Public Key
    unsigned char encrypted_aes_key[256];
    int encrypted_len = rsa_encrypt_with_public_key("public.pem", aes_key, AES_KEY_SIZE, encrypted_aes_key);
    send(client_socket, encrypted_aes_key, encrypted_len, 0);
    printf("Sent encrypted AES session key.\n");

    // Send IV to the client
    send(client_socket, iv, AES_BLOCK_SIZE, 0);
    printf("Sent AES IV to client.\n");

    // Send Encrypted Exam Data
    char exam_data[] = "Question 1: What is 2 + 2?";
    unsigned char encrypted_exam[BUFFER_SIZE];
    int exam_len = aes_encrypt(aes_key, exam_data, strlen(exam_data), encrypted_exam, iv);
    send(client_socket, encrypted_exam, exam_len, 0);
    printf("Sent encrypted exam data.\n");

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);

    printf("Server is running on port %d...\n", PORT);

    while (1) {
        int *client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_size);
        pthread_t thread;
        pthread_create(&thread, NULL, client_handler, client_socket);
        pthread_detach(thread);
    }

    close(server_socket);
    return 0;
}

