

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
#define PORT 1998
#define BUFSIZE 8192

pthread_mutex_t	mutex_ndone = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	mutex_n_sending = PTHREAD_MUTEX_INITIALIZER;
int numberClient=0;
struct Client{
    int IpAddr;
    long port;
    char listFile[100][100];
    long listFileSize[100];
    int numberFile;
};

struct Client client[100];

void sendAccept(int *fd) {
    int message = 1;
    write(*fd, &message, sizeof(message));
}

void recivelistFile(int *fd) {
    int n_bytes;
    int nFile, fileLen;
    char fileName[100];
    long port;
    long fileSize;
    
    read(*fd,&port,sizeof(port));
    printf("listening port on client: %ld\n",port);
    // struct sockaddr_in addr;
	// socklen_t addr_size = sizeof(struct sockaddr_in);
	// int *res = getpeername(*fd, (struct sockaddr *)&addr, &addr_size);
    int flag = 1;
    for(int i=0;i<numberClient;i++){
        if(client[i].port==port){
            n_bytes = read(*fd, &nFile, sizeof(int));
            printf("Number of file : %d\n", nFile);
            client[i].numberFile=nFile;
            for (int j = 0; j < nFile; j++) {
                int fileNameLength;
                read(*fd,&fileNameLength,sizeof(fileNameLength));
                n_bytes = read(*fd, &client[i].listFile[j], fileNameLength);
                client[i].listFile[j][n_bytes]=0;
                printf("filename: %s\n", client[i].listFile[j]);
                n_bytes = read(*fd, &fileSize, sizeof(fileSize));
                printf("fileLength: %ld\n", fileSize);
                
                client[i].listFileSize[j]=fileSize;
                //n_bytes = read(fd, fileName, fileLen);
                //fileName[n_bytes-1] = '\0';

                
                // printf("infor file : %s %d\n", fileName, fileSize);
            }
            flag = 0;
            break;
        }
    }
    if(flag==1){
    client[numberClient-1].port=port;
    n_bytes = read(*fd, &nFile, sizeof(int));
    printf("Num of file : %d\n", nFile);
    client[numberClient-1].numberFile=nFile;
    for (int i = 0; i < nFile; i++) {
        int fileNameLength;
        read(*fd,&fileNameLength,sizeof(fileNameLength));
        n_bytes = read(*fd, &client[numberClient-1].listFile[i], fileNameLength);
        client[numberClient-1].listFile[i][n_bytes]=0;
        printf("name %d, %d, %s\n", i, n_bytes, client[numberClient-1].listFile[i]);
        n_bytes = read(*fd, &fileSize, sizeof(fileSize));
        printf("fileLength %ld\n", fileSize);
        
        client[numberClient-1].listFileSize[i]=fileSize;
        //n_bytes = read(fd, fileName, fileLen);
        //fileName[n_bytes-1] = '\0';

        
        // printf("infor file : %s %d\n", fileName, fileSize);
    }
    }
}

void recvRequest(int *fd){
    int fileNameLength;
    char fileName[100];
    int numberRightClient=0;
    int listIP[100];
    int port[100];
    long fileSize;
    read(*fd,&fileNameLength,sizeof(fileNameLength));
    read(*fd,&fileName,fileNameLength);
    fileName[fileNameLength]=0;
    // printf("file name length: %d",fileNameLength);
    printf("client want to down load file: %s\n",fileName);
    for(int i=0;i<numberClient;i++){
        for(int j=0;j<client[i].numberFile;j++){
            // printf("file in list: %s\n",client[i].listFile[j]);
            if(strcmp(fileName,client[i].listFile[j])==0){
                numberRightClient++;
                listIP[numberRightClient-1]=client[i].IpAddr;
                port[numberRightClient-1]=client[i].port;
                fileSize=client[i].listFileSize[j];
                break;
            }
        }
    }
    if(numberRightClient>0){
        int type = 4;
        write(*fd,&type,sizeof(type));
        write(*fd,&fileSize,sizeof(fileSize));
        write(*fd,&numberRightClient,sizeof(numberRightClient));
        for(int i=0;i<numberRightClient;i++){
            write(*fd,&listIP[i],sizeof(listIP[i]));
            write(*fd,&port[i],sizeof(port[i]));
        }
    }else{
        int type = 7;
        int errorCode = 1;
        write(*fd,&type,sizeof(type));
        write(*fd,&errorCode,sizeof(errorCode));
    }
}



void * process(void * file_description) {
    int fd = *((int *) file_description);
    int n_bytes, message;

    sendAccept(&fd);   
    
   
  
    while(n_bytes = read(fd, &message, sizeof(int))>0){
    printf("new message, type = %d\n", message);
    if (message == -1 || n_bytes==0) {
        printf("* Disconnect client \n");
        return NULL;
    }  else 
    if (message == 2) {
        printf("client want to send list file\n");
        
        recivelistFile(&fd);
        sendAccept(&fd);
    } else 
    if (message == 6){
        // printf("Message : %d\n", message);
        recvRequest(&fd);
    } else {
        // printf("Message : %d\n", message);
    }
    }

    if(n_bytes==0){
        printf("one client has closed!\n");
    }
    
    
    return NULL;
}

int main(int argc, char **argv) {
    struct sockaddr_in server, cli;
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    char str[100];
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("172.20.10.11");
    bind(serverfd, (struct sockaddr*) &server, sizeof(server));
    listen(serverfd, 100);

    socklen_t clisize;
    pthread_t tid;
    int * iptr;
    while (1) {   
        iptr = malloc(sizeof(int));
        clisize= sizeof(cli);
        *iptr = accept(serverfd, (struct sockaddr*) &cli, &clisize);
        printf("\nREQUEST FROM %s PORT %d \n",inet_ntop(AF_INET,&cli.sin_addr,str,sizeof(str)),htons(cli.sin_port));
        numberClient++;
        client[numberClient-1].IpAddr=cli.sin_addr.s_addr;
        pthread_create(&tid, NULL, &process, (void *) iptr);
        
        

    }
    
    return 0;
}
