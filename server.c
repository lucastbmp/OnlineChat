#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h> // Internet protocol family
#include <arpa/inet.h> // internet operations
#include <string.h>



struct connected_socket{
    int clientFD;
    struct sockaddr_in clientAddress;
};


struct connected_socket* acceptIncomingConnection(int serverSocketFD){
    struct sockaddr_in clientAddress;
    struct sockaddr *p_client_sockaddr = (struct sockaddr*) &clientAddress;
    unsigned int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverSocketFD, p_client_sockaddr, &clientAddressSize);

    struct connected_socket* p_connected_socket = malloc(sizeof(struct connected_socket));
    p_connected_socket->clientAddress = clientAddress;
    p_connected_socket->clientFD = clientSocketFD;

    if(clientSocketFD != 0){
        exit(1);
    }

    return p_connected_socket;
};


int main(){
    // Return a file descriptor
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);

    // Creation of IPV4 tcp socket
    char* ip = "127.0.0.1";
    unsigned int port = 5000;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port); // htons = host to network short, gets the bytes of the integer and reverses it
    inet_pton(AF_INET, ip,  &address.sin_addr.s_addr); // Transform 2nd argument charset to int of 4 bytes and put in the 3rd argument || pton = presentation to network
    address.sin_addr.s_addr; // IP
    struct sockaddr *p_sockaddr = (struct sockaddr *) &address;

    if(bind(socketFD, p_sockaddr, sizeof address) == 0){
        printf("Sucessfuly binded socket to %s:%u!\n", ip, port);
    }
    if(listen(socketFD, 5) == 0){
        printf("listening...");
    }

    struct connected_socket* clientSocket = acceptIncomingConnection(socketFD);

    char messages[1024];
    char commands[100];
    char username[100];
    do{
        fgets(commands, sizeof(commands), stdin);
        commands[strcspn(commands, "\n")] = 0;

        int username_size = recv(socketFD, username, 100, 0);
        int msg_size = recv(socketFD, messages, 1024, 0);

        if(msg_size > 0){
            printf("%s: %s", username, messages);
            memset(username, 0, 100);
            memset(messages, 0, 1024);
        }
    }while(strcmp(commands, "exit") != 0);
    shutdown(socketFD, 0);

    return 0;
}
