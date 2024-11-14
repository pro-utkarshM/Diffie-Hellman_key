#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8082

// Function to print buffer as a hexadecimal string
void print_hex(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    int sockfd, newsockfd, n;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;

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

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    printf("Server is running and waiting for clients on %s:%d...\n", SERVER_IP, PORT);

    // Accept incoming connection
    clilen = sizeof(client_addr);
    newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &clilen);
    if (newsockfd < 0) {
        perror("Error accepting connection");
        exit(1);
    }
    printf("Client connected.\n");

    // Read encrypted message from client
    memset(buffer, 0, sizeof(buffer));
    n = read(newsockfd, buffer, sizeof(buffer));
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    printf("Received Encrypted Message: ");
    print_hex((unsigned char*)buffer, n);  // Print encrypted message in hex

    close(newsockfd);
    close(sockfd);
    return 0;
}
