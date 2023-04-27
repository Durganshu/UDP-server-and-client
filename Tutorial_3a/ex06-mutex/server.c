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
#include <pthread.h>

#include "server.h"

#define MESSAGE_LENGTH 1000
#define NCLIENTS 5
#define N 2

int port, sockfd, active_clients = 0;
struct sockaddr_in server_addr, client_addr;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void create_socket();
void *handle_client(void *arg);

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Error: Missing arguments. Exiting!!!\n");
        return 2;
    } else if (argc > 2){
        printf("Error: Too many arguments. Exiting!!!\n");
        return 2;
    }
    sscanf(*(argv+1), "%d", &port);
    create_socket();
    pthread_t threads[NCLIENTS];
    while (1) {
        if (active_clients < N) {
            int *arg = malloc(sizeof(*arg));
            *arg = sockfd;
            if (pthread_create(&threads[active_clients], NULL, handle_client, arg)) {
                fprintf(stderr, "Error creating thread\n");
                return 1;
            }
            pthread_detach(threads[active_clients]);
            active_clients++;
        }
    }
    close(sockfd);
    return 0;
}

void create_socket(){
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void *handle_client(void *arg) {
    int sockfd = *(int *)arg;
    free(arg);
    char recv_buffer[MESSAGE_LENGTH];
    socklen_t len = sizeof(client_addr);
    int n = recvfrom(sockfd, (char *)recv_buffer, MESSAGE_LENGTH, MSG_WAITALL, (struct sockaddr *)&client_addr, &len);
    recv_buffer[n] = '\0';
    printf("\n(Thread# %ld) Received this message from the client :\n%s\n", (long)pthread_self(), recv_buffer);
    //Send reply
    sendto(sockfd, (const char *)recv_buffer, MESSAGE_LENGTH, MSG_CONFIRM, (const struct sockaddr *)&client_addr, len);
    printf("(Thread# %ld) Sent the received message back to client.\n", (long)pthread_self());
    pthread_mutex_lock(&mutex);
    active_clients--;
    pthread_mutex_unlock(&mutex);
    close(sockfd);
    return NULL;
}