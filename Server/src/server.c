

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
void sendAccept(int *fd) {
    int message = 1;
    write(*fd, &message, sizeof(message));
}

void recivelistFile(int *fd) {
    int n_bytes;
    int nFile, fileSize, fileLen;
    char fileName[32];
    n_bytes = read(*fd, &nFile, sizeof(int));
    printf("Num of file : %d\n", nFile);
    
    for (int i = 0; i < nFile; i++) {
        n_bytes = read(*fd, &fileName, 32);
        printf("name %d %d %s\n", i, n_bytes, fileName);
        n_bytes = read(*fd, &fileSize, sizeof(int));
        printf("len %d %d\n", n_bytes, fileSize);
        
        //n_bytes = read(fd, fileName, fileLen);
        //fileName[n_bytes-1] = '\0';

        
        // printf("infor file : %s %d\n", fileName, fileSize);
    }
    
}

void * process(void * file_description) {
    int fd = *((int *) file_description);
    int n_bytes, message;

    sendAccept(&fd);   
    
    LOOP:
  
    n_bytes = read(fd, &message, sizeof(int));
    printf("new message %d\n", message);
    if (message == -1) {
        printf("* Disconnect client port: %d.\n", listPort[fd]);
        return NULL;
    }  else 
    if (message == 2) {
        printf("client want to send list file\n");
        sendAccept(&fd);
        recivelistFile(&fd);
    } else 
    if (message == 3){
        printf("Message : %d\n", message);
    } else {
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
        pthread_create(&tid, NULL, &process, (void *) iptr);
        listPort[*iptr] = htons(client.sin_port);
        

    }
    
    return 0;
}
