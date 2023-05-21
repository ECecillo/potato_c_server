// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int client_socket = 0; long valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    // Buffer to store the message sent by the server.
    char buffer[1024] = {0};

    // Creating socket file descriptor
    // socket(domain, type, protocol)
    // domain: AF_INET (IPv4 protocol)  
    // type: SOCK_STREAM (TCP)
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    // Allocate mem for create serv_addr
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    // Define the address based on the spec of our TCP/IP socket.
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    // Has a client I send a message to the server.
    send(client_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    // I wait for the server to send me a message and print it after.
    valread = read( client_socket , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
