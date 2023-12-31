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

#include "server.h"

int main(int argc, char *argv[]){

    if (argc < 2){
        printf("Error: Missing arguments. Exiting!!!\n");
        return 2;
    }
    else if(argc > 2){
        printf("Error: Too many arguments. Exiting!!!\n");
        return 2;
    }
    
    int port;
    sscanf(*(argv+1), "%d", &port);

    int sockfd;
	char recv_message[1000];
	struct sockaddr_in server_addr, client_addr;
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));
	
	// Filling server information
	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&server_addr,
			sizeof(server_addr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	socklen_t len;

	len = sizeof(client_addr); //len is value/result

	while (1)
        {

          int n = recvfrom(sockfd, (char *)recv_message, 1000,
				MSG_WAITALL, ( struct sockaddr *) &client_addr,
				&len);


          recv_message[n] = '\0';

          printf("Client : %s\n", recv_message);
          //printf("SEND : ");
         // gets(send_data);
			sendto(sockfd, (const char *)recv_message, strlen(recv_message),
			MSG_CONFIRM, (const struct sockaddr *) &client_addr,
			len);
	
			printf("\n Sent the received message back to client.\n");

          fflush(stdout);

        }
	   
}

// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// #include <stdlib.h>

// int main(int argc, char *argv[])
// {
//         int sock;
//         int addr_len, bytes_read;
//         char recv_data[1024],send_data[1024];
//         struct sockaddr_in server_addr , client_addr;

// 		int port;
// 	    sscanf(*(argv+1), "%d", &port);

//         if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
//             perror("Socket");
//             exit(1);
//         }

//         server_addr.sin_family = AF_INET;
//         server_addr.sin_port = htons(port);
//         server_addr.sin_addr.s_addr = INADDR_ANY;
//         bzero(&(server_addr.sin_zero),8);

//         if (bind(sock,(struct sockaddr *)&server_addr,
//             sizeof(struct sockaddr)) == -1)
//         {
//             perror("Bind");
//             exit(1);
//         }

//         addr_len = sizeof(struct sockaddr);

//         printf("\nUDPServer Waiting for client on port 5000");
//         fflush(stdout);

//         int count = 0,i;
//         char a[20][10],d[20][10];
//         strcpy(a[0],"A");
//         strcpy(a[1],"B");
//         strcpy(a[2],"C");
//         strcpy(a[3],"D");
//         strcpy(d[0],"123");
//         strcpy(d[1],"124");
//         strcpy(d[2],"100");
//         strcpy(d[3],"99");

//         while (1)
//         {

//           bytes_read = recvfrom(sock,recv_data,1024,0,
//                             (struct sockaddr *)&client_addr, &addr_len);


//           recv_data[bytes_read] = '\0';

//           printf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr),
//                                        ntohs(client_addr.sin_port));
//           printf("%s", recv_data);
//           printf("SEND : ");
//          // gets(send_data);

//              int p = 0;
//                  for(i=0;i<4;i++)
//                  {
//                         if(strcmp(recv_data,d[i]) == 0)
//                         {
//                              strcpy(send_data,a[i]);p=1;
//                         }
//                  }
//                  if(p == 0)
//                  strcpy(send_data,"No one on that role.");

//           sendto(sock,send_data,strlen(send_data),0,(struct sockaddr *)&client_addr,sizeof(struct sockaddr));

//           fflush(stdout);

//         }
//         return 0;
// }