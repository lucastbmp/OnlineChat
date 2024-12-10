#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h> // Internet protocol family
#include <arpa/inet.h> // internet operations

int main(){
    // Return a file descriptor
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);

    char* ip = "127.0.0.1";
    unsigned int port = 5000;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port); // htons = host to network short, gets the bytes of the integer and reverses it
    inet_pton(AF_INET, ip,  &address.sin_addr.s_addr); // Transform 2nd argument charset to int of 4 bytes and put in the 3rd argument || pton = presentation to network
    address.sin_addr.s_addr; // IP
    struct sockaddr *p_sockaddr = (struct sockaddr *) &address;

    // Connect needs a file descriptor
    int conn = connect(socketFD, p_sockaddr, sizeof address);
    if(conn == 0){
        printf("Sucessfuly connected socket to %s:%u!\n", ip, port);
    }
    return 0;
}
