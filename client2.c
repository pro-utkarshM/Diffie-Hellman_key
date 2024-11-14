// client2.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8081

// Function to encrypt message using Caesar cipher
void encrypt_message(char *message, char *encrypted_message) {
    for (int i = 0; message[i] != '\0'; i++) {
        encrypted_message[i] = message[i] + 3;  // Caesar Cipher encryption (shift by 3)
    }
    encrypted_message[strlen(message)] = '\0';  // Null-terminate
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[1024], encrypted_message[1024];
    int n;

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

    // Get the message to send
    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;  // Remove newline

    // Encrypt the message
    encrypt_message(message, encrypted_message);

    // Send encrypted message
    n = write(sockfd, encrypted_message, strlen(encrypted_message));
    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    printf("Encrypted message sent: %s\n", encrypted_message);

    close(sockfd);
    return 0;
}
