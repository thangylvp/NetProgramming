
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>

#define N 512
#define PORT 1236

bool checkAccept(int *fd) {
    int n_bytes, message;
    n_bytes = read(*fd, &message, sizeof(int));

    if (message == 1) return true;
    return false;
}

void sendMessage(int *fd, int message) {
    write(*fd, &message, sizeof(message));
}

int getFileSize(char* fileName) {
    FILE* file = fopen(fileName, "r");
    fseek(file, 0L, SEEK_END);
    int file_size = (int)ftell(file);
    fclose(file);
    return file_size;
}

void getListClient(int *fd, char* fileName) {
    write(*fd, fileName, sizeof(fileName));
    int n_bytes;
    int numClient;
    char ip[32];
    int port;
    n_bytes = read(*fd, &numClient, sizeof(int));   
    printf("There are %d client\n", numClient);
    for (int i = 0; i < numClient; i++) {
        n_bytes = read(*fd, &ip, 32);
        n_bytes = read(*fd, &port, sizeof(int));
        printf("address ip %s port %d\n", ip, port);
    }
}
void sendListFile(int *fd) {
    DIR *d;
    struct dirent *dir;
    int numFile = 0, fileSize;
    char fileName[32];
    d = opendir("data/");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strlen(dir->d_name) > 3) numFile++;
        }
        closedir(d);
    }

    // printf("num file: %d\n", numFile);
    sendMessage(fd, numFile);
    d = opendir("data/");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strlen(dir->d_name) > 3) {

                sprintf(fileName, "%s%s", "data/", dir->d_name);
                fileSize = getFileSize(fileName);
                sprintf(fileName, "%s", dir->d_name);
                // printf("link file %s file size %d\n", fileName, fileSize);
                //write(*fd, sizeof(dir->d_name), sizeof(int));
                write(*fd, fileName, sizeof(fileName));
                write(*fd, &fileSize, sizeof(int));
            }
        }
        // closedir(d);
    }
    
    return;
}
int process(int fd) {
    // char file_name[N];
    int n_bytes, message;

    
    
    
    if (checkAccept(&fd) == true) {
        printf("Accept Connection\n");
    } else {
        printf("Cannot connect to server!\n");
        return 0;
    }

    message = 2;
    sendMessage(&fd, message);
    if (message == 2) {
        while (checkAccept(&fd) == false) {}
        // printf("Server ready -> send list file\n");
        sendListFile(&fd);
        if (checkAccept(&fd) == true) {
            printf("Send list file success\n");
        } else {
            printf("Cannot send list file!\n");
            return 0;
        }

    }

    printf("----------------------\n\n");
    LOOP:
    printf("\n\n");
    printf("Type -1 to close connection\n");
    printf("Type 2 to send list file to server\n");
    printf("Type 6 to send file need download to server\n");
    printf("Choose your message(-1, 2, 3,...): ");
    scanf("%d", &message);

    sendMessage(&fd, message);
    
    if (message == -1) {
        printf("Close connection\n");
        return 0;
    } else 
    if (message == 2) {
        while (checkAccept(&fd) == false) {}
        // printf("Server ready -> send list file\n");
        sendListFile(&fd);
        if (checkAccept(&fd) == true) {
            printf("Send list file success\n");
        } else {
            printf("Cannot send list file!\n");
            return 0;
        }

    } else 
    if (message == 6) {
        printf("Choose your file name: ");
        char fileName[32];
        scanf("%s", fileName);
        getListClient(&fd, fileName);
    }

    
    

    goto LOOP;
    return 1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: %s IP_ADDRESS\n", argv[0]);
    }

    struct sockaddr_in server;
    int srvfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    
    inet_pton(AF_INET, argv[1], &server.sin_addr);
    connect(srvfd, (struct sockaddr*) &server, sizeof(server));
    
    if (srvfd >= 0) {
        while (1) {
            if (process(srvfd) == 0) {
                break;
            }
        }
    }
    close(srvfd);

    return 0;
}
