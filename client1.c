// client1.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080  // Server's port for Client 1
#define SERVER "127.0.0.1"  // Server IP address

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    long long p = 23, g = 5, a = 6;  // Prime and generator for Diffie-Hellman
    long long A = power(g, a, p);  // Public key

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER);
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Send public key
    sprintf(buffer, "%lld", A);
    write(sockfd, buffer, strlen(buffer));

    // Receive public key from other client
    memset(buffer, 0, sizeof(buffer));
    read(sockfd, buffer, sizeof(buffer)-1);
    printf("Received public key: %s\n", buffer);

    close(sockfd);
    return 0;
}
