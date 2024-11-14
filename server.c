// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT1 8080  // Port for Client 1
#define PORT2 8081  // Port for Client 2

// Function to handle communication with a client
void handle_client(int client_socket) {
    char buffer[1024];
    int n;
    
    // Receive the encrypted message
    memset(buffer, 0, sizeof(buffer));
    n = read(client_socket, buffer, sizeof(buffer)-1);
    if (n < 0) {
        perror("Error reading from socket");
        return;
    }

    printf("Received Encrypted Message: %s\n", buffer);

    // Decrypt the message using Caesar cipher (For simplicity)
    char decrypted_message[1024];
    for (int i = 0; buffer[i] != '\0'; i++) {
        decrypted_message[i] = buffer[i] - 3;  // Caesar Cipher decryption (shift by 3)
    }
    decrypted_message[strlen(buffer)] = '\0';  // Null-terminate

    printf("Decrypted Message: %s\n", decrypted_message);
}

int main() {
    int sockfd1, sockfd2, client_socket1, client_socket2;
    struct sockaddr_in server_addr1, server_addr2, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket for Client 1
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) {
        perror("Error opening socket for Client 1");
        exit(1);
    }

    // Set up server address for Client 1
    memset(&server_addr1, 0, sizeof(server_addr1));
    server_addr1.sin_family = AF_INET;
    server_addr1.sin_addr.s_addr = INADDR_ANY;
    server_addr1.sin_port = htons(PORT1);

    // Bind and listen for Client 1
    if (bind(sockfd1, (struct sockaddr*)&server_addr1, sizeof(server_addr1)) < 0) {
        perror("Error binding to Client 1 socket");
        exit(1);
    }

    listen(sockfd1, 5);

    // Create socket for Client 2
    sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd2 < 0) {
        perror("Error opening socket for Client 2");
        exit(1);
    }

    // Set up server address for Client 2
    memset(&server_addr2, 0, sizeof(server_addr2));
    server_addr2.sin_family = AF_INET;
    server_addr2.sin_addr.s_addr = INADDR_ANY;
    server_addr2.sin_port = htons(PORT2);

    // Bind and listen for Client 2
    if (bind(sockfd2, (struct sockaddr*)&server_addr2, sizeof(server_addr2)) < 0) {
        perror("Error binding to Client 2 socket");
        exit(1);
    }

    listen(sockfd2, 5);

    printf("Server is running and waiting for clients...\n");

    // Accept connections from both clients
    client_socket1 = accept(sockfd1, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket1 < 0) {
        perror("Error accepting connection from Client 1");
        exit(1);
    }

    printf("Connected to Client 1...\n");

    client_socket2 = accept(sockfd2, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket2 < 0) {
        perror("Error accepting connection from Client 2");
        exit(1);
    }

    printf("Connected to Client 2...\n");

    // Handle communication with both clients
    handle_client(client_socket1);  // Handle Client 1 message
    handle_client(client_socket2);  // Handle Client 2 message

    close(client_socket1);
    close(client_socket2);
    close(sockfd1);
    close(sockfd2);

    return 0;
}
