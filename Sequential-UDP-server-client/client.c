#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "client.h"

int main(int argc, char *argv[]){

    if (argc < 3){
        printf("Error: Missing arguments. Exiting!!!\n");
        return 2;
    }
    else if(argc > 3){
        printf("Error: Too many arguments. Exiting!!!\n");
        return 2;
    }
    
    char sent_message[1000];

    printf("Input your message (end the message with ;):\n");
    scanf("%[^;]s", sent_message);
    //printf("\nReceived message:\n%s", sent_message );
    char *ip_address = NULL;
    ip_address = *(argv+1);
    //printf("%s", ip_address);
    int port;
    sscanf(*(argv+2), "%d", &port);
    //printf("Extraxted port: %d\n", port);

    int sockfd;
    struct sockaddr_in ipv4_address;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&ipv4_address, 0, sizeof(ipv4_address));
	
    //https://linuxhint.com/c-inet-pton-function/
    // char buffer  [INET_ADDRSTRLEN] = "192.168.1.1";
    struct sockaddr_in *addr_in;

    ipv4_address.sin_family = AF_INET;
    ipv4_address.sin_port = htons(port);
    ipv4_address.sin_addr.s_addr = inet_addr((char * )ip_address);


    int sent_bytes = sendto(sockfd, (const char *)sent_message, 
        strlen(sent_message)+1, MSG_CONFIRM, 
        (struct sockaddr*)&ipv4_address, sizeof(ipv4_address));

    printf("%d bytes sent to server\n", sent_bytes);

    return 0;
}
//https://www.geeksforgeeks.org/udp-server-client-implementation-c/