#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {

    const char* msg = "Hello, server!";
    ssize_t valread;
    char buffer[1024] = { 0 };
    
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 

    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error when opening socket\n");
        exit(1);
    }
    printf("Socket created\n");

    int ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0) {
        printf("Error when connecting socket\n");
        exit(1);
    }
    printf("Connected\n");

    while(1) {
        sleep(2);
        ret = send(sockfd, msg, strlen(msg), 0);
        if (ret < 0) {
            printf("Error when sending msg\n");
        } else {
            valread = read(sockfd, buffer, BUFFER_SIZE - 1);
            printf("Message recieved: \n%s\n", buffer);
            memset(buffer, 0, BUFFER_SIZE);
        }
    }

    close(sockfd);
    
    return 0;
}