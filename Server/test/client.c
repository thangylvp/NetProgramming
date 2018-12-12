
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define N 512
#define PORT 1235

int receiveFile(int fd) {
    // char file_name[N];
    int n_bytes, message;
    printf("Choose your message: ");
    scanf("%d", &message);
    write(fd, &message, sizeof(message));
    
    if (message == -1) {
        printf("\nclose!\n");
        return 0;
    }


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
            if (receiveFile(srvfd) == 0) {
                break;
            }
        }
    }
    close(srvfd);

    return 0;
}
