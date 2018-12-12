

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define N 512
#define PORT 1235


pthread_mutex_t	mutex_ndone = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	mutex_n_sending = PTHREAD_MUTEX_INITIALIZER;

void * sendFile(void * file_description) {
    int fd = *((int *) file_description);
    int n_bytes;
    int message;

    LOOP:
    n_bytes = read(fd, &message, sizeof(int));
    
    if (message == -1) {
        printf("* Disconnect client ID: %d.\n", fd);
        return NULL;
    }

    
    

    goto LOOP;
    
    return NULL;
}

int main(int argc, char **argv) {
    struct sockaddr_in server, client;
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(serverfd, (struct sockaddr*) &server, sizeof(server));
    listen(serverfd, 100);

    socklen_t clisize;
    pthread_t tid;
    int * iptr;
    while (1) {   
        iptr = malloc(sizeof(int));
        clisize= sizeof(client);
        *iptr = accept(serverfd, (struct sockaddr*) &client, &clisize);
        pthread_create(&tid, NULL, &sendFile, (void *) iptr);
        printf("* New client. ID:  %d\n", *iptr);
    }
    
    return 0;
}
