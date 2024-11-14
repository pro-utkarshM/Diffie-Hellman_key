#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8082

// Function to encrypt a message using AES
void encrypt_message(const unsigned char *message, unsigned char *encrypted_message, unsigned char *aes_key) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0;
    int encrypted_len = 0;

    // Initialize encryption context
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, aes_key, NULL)) {
        perror("Error initializing encryption");
        exit(1);
    }

    // Perform encryption
    if (1 != EVP_EncryptUpdate(ctx, encrypted_message, &encrypted_len, message, strlen((char*)message))) {
        perror("Error encrypting message");
        exit(1);
    }

    // Finalize encryption (handles padding)
    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_message + encrypted_len, &len)) {
        perror("Error finalizing encryption");
        exit(1);
    }
    encrypted_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

int main() {
    int sockfd, n;
    struct sockaddr_in server_addr;
    unsigned char aes_key[AES_BLOCK_SIZE] = {0};  // Example static AES key (replace with shared key)
    unsigned char encrypted_message[1024];
    char message[1024];

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

    // Get message to send from user
    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;  // Remove newline character

    // Encrypt the message
    encrypt_message((unsigned char*)message, encrypted_message, aes_key);

    // Send encrypted message to server
    n = write(sockfd, encrypted_message, strlen((char*)encrypted_message));
    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    printf("Encrypted message sent: ");
    for (int i = 0; i < strlen((char*)encrypted_message); i++) {
        printf("%02x", encrypted_message[i]);
    }
    printf("\n");

    close(sockfd);
    return 0;
}
