#include <stdio.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <zconf.h>
#include <memory.h>

#define MAX_LINE 2048
#define SERVER_PORT 9999

void echo(FILE *fp, int socketFd);

int
main(int argc, char *argv[]) {
    int socketFd;
    struct sockaddr_in serverAddress;

    if (argc != 2) {
        perror("usage: echo_client <IPAddress>");
    }

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9999);

    inet_pton(AF_INET, argv[1], &serverAddress.sin_addr);

    /**
     * client 端socket发送连接请求
     */
    if (connect(socketFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connect Error !");
        exit(1);
    } else {
        printf("Connected Successfully !\n");
        // 处理连接过程
        echo(stdin, socketFd);
        exit(0);
    }
}

void echo(FILE *fp, int socketFd) {
    char sendLine[MAX_LINE];
    char receiveLine[MAX_LINE];
    while (fgets(sendLine, MAX_LINE, fp)) {
        write(socketFd, sendLine, strlen(sendLine));
        if (read(socketFd, receiveLine, MAX_LINE) == 0) {
            perror("Client : Server terminated prematurely !");
            exit(-1);
        }
        fputs(receiveLine, stdout);
    }
}
