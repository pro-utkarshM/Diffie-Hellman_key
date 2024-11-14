#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8082

// Function to decrypt a message using AES
void decrypt_message(const unsigned char *encrypted_message, unsigned char *decrypted_message, unsigned char *aes_key, int encrypted_len) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0;
    int decrypted_len = 0;

    // Initialize decryption context
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, aes_key, NULL)) {
        perror("Error initializing decryption");
        exit(1);
    }

    // Perform decryption
    if (1 != EVP_DecryptUpdate(ctx, decrypted_message, &decrypted_len, encrypted_message, encrypted_len)) {
        perror("Error decrypting message");
        exit(1);
    }

    // Finalize decryption (handles padding)
    if (1 != EVP_DecryptFinal_ex(ctx, decrypted_message + decrypted_len, &len)) {
        perror("Error finalizing decryption");
        exit(1);
    }
    decrypted_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

int main() {
    int sockfd, n;
    struct sockaddr_in server_addr;
    unsigned char aes_key[AES_BLOCK_SIZE] = {0};  // Example static AES key (replace with shared key)
    unsigned char encrypted_message[1024];
    unsigned char decrypted_message[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Receive encrypted message from server
    n = read(sockfd, encrypted_message, sizeof(encrypted_message));
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    // Decrypt the received message
    decrypt_message(encrypted_message, decrypted_message, aes_key, n);

    // Print the decrypted message
    printf("Decrypted message: %s\n", decrypted_message);

    close(sockfd);
    return 0;
}
