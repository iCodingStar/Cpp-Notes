#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include <errno.h>

#define LISTENQ 5
#define MAXLINE 2048
#define SERV_PORT 9999
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct sockaddr SA;

void
str_cli(FILE *fp, int socketFd) {
    int maxFd;
    fd_set readSet;
    char sendline[MAXLINE], recvline[MAXLINE];

    for (;;) {
        // FD_ZERO(&readSet);
        // 添加对fp文件的监听
        FD_SET(fileno(fp), &readSet);
        // 添加对socket时间的监听
        FD_SET(socketFd, &readSet);
        // 计算最大的文件描述符
        maxFd = max(fileno(fp), socketFd) + 1;
        select(maxFd, &readSet, NULL, NULL, NULL);
        //处理当前的socket读事件
        if (FD_ISSET(socketFd, &readSet)) {    /* socket is readable */
            int readNum;
            if ((readNum = read(socketFd, recvline, MAXLINE)) == 0) {
                perror("str_cli: server terminated prematurely");
                exit(1);
            } else if (readNum < 0) {
                perror("str_cli: read error ");
                perror(errno);
                exit(1);
            } else {
                recvline[readNum] = '\0';
                fputs(recvline, stdout);
            }

        }

        // 处理文件流的读取事件
        if (FD_ISSET(fileno(fp), &readSet)) {  /* input is readable */
            if (fgets(sendline, MAXLINE, fp) == NULL)
                return;        /* all done */
            write(socketFd, sendline, strlen(sendline));
        }
    }
}

int
main(int argc, char **argv) {
    int socketFd;
    struct sockaddr_in serverAddress;

    if (argc != 2) {
        perror("usage: tcpcli <IPaddress>");
        exit(1);
    }
    socketFd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &serverAddress.sin_addr);

    connect(socketFd, (SA *) &serverAddress, sizeof(serverAddress));
    str_cli(stdin, socketFd);        /* do it all */

    exit(0);
}