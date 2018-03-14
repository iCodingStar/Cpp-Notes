//
// Created by star on 18-3-13.
//
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <zconf.h>
#include <errno.h>


#define MAXLINE 2048
#define SA struct sockaddr
#define SERVER_PORT 9999
#define LISTEN 5

void echo(int connectFd);

int
main(int argc, char *argv[]) {

    int listenFd, connectFd;
    pid_t childPid;
    socklen_t childLength;
    struct sockaddr_in clientAddress, serverAddress;
    // 创建一个基于IPv4的Socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(SERVER_PORT);
    // 绑定服务端的地址
    bind(listenFd, (SA *) &serverAddress, sizeof(serverAddress));

    // 监听服务端地址，等待连接
    listen(listenFd, LISTEN);

    for (;;) {
        childLength = sizeof(clientAddress);
        // server 端接收到连接请求
        connectFd = accept(listenFd, (SA *) NULL, NULL);

        printf("successful connected ...\n");

        if ((childPid = fork()) == 0) { ///
            close(listenFd); // 将listenFd减1
            echo(connectFd); // process the request
            _exit(0);
        }
        // parent close connected df
        close(connectFd);
    }
}

void echo(int connectFd) {
    ssize_t n;
    char buffer[MAXLINE];

    again:
    while ((n = read(connectFd, buffer, MAXLINE)) > 0) {
        printf("write back to the client : %s\n", buffer);
        buffer[n] = '\n';
        write(connectFd, buffer, n);
    }
    // 如果只是被系统中断，可以继续
    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        perror("echo : read error !");
        _exit(1);
    }
}