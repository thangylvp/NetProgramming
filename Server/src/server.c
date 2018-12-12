

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
int listPort[100];

void * sendFile(void * file_description) {
    int fd = *((int *) file_description);
    int n_bytes;
    int message;

    message = 1;
    write(fd, &message, sizeof(message));
    
    LOOP:

    

    n_bytes = read(fd, &message, sizeof(int));
    
    if (message == -1) {
        printf("* Disconnect client port: %d.\n", listPort[fd]);
        return NULL;
    }  else {
        printf("Message : %d\n", message);
    }

    
    

    goto LOOP;
    
    return NULL;
}

int main(int argc, char **argv) {
    struct sockaddr_in server, client;
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    char str[100];
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
        printf("\nREQUEST FROM %s PORT %d \n",inet_ntop(AF_INET,&client.sin_addr,str,sizeof(str)),htons(client.sin_port));
        pthread_create(&tid, NULL, &sendFile, (void *) iptr);
        listPort[*iptr] = htons(client.sin_port);
        

    }
    
    return 0;
}
