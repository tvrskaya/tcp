#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <iostream>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
    int sockfd, connfd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    ssize_t valread;
    char buffer[1024] = { 0 };

    const char* okMsg = "Msg accepted";
    const char* failMsg = "Msg fail";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) { 
        printf("Socket fail\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Socket created\n");

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(PORT); 

    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind fails");
        exit(EXIT_FAILURE);
    }

    // start listening for incoming connections
    if (listen(sockfd, 3) < 0) {
        perror("listening failed");
        exit(EXIT_FAILURE);
    }

    // print a message showing that the server is listening on a port
    printf("Server listening on port %d\n", PORT);

    if ((connfd = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Connection accepted\n");


    while (1)
    {
        valread = read(connfd, buffer, BUFFER_SIZE - 1);
        if (valread > 0) {
            printf("Message recieved: \n%s\n", buffer);
            send(connfd, okMsg, strlen(okMsg), 0);
        } else {
            printf("Error while recieving message\n");
            send(connfd, failMsg, strlen(failMsg), 0);
        }
    }
    
    close(connfd);
    close(sockfd);
    return 0;
}