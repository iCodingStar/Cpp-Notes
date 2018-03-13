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

}

int
main(int argc, char **argv) {
    int i, maxi, maxFd, listenFd, connectFd, socketFd;
    int readyNum, client[FD_SETSIZE];
    ssize_t readNum;
    fd_set readSet, allSet;
    char buffer[MAXLINE];
    socklen_t clientLength;
    struct sockaddr_in clientAddress, serverAddress;
    // 创建一个socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    // 初始化服务端地址信息
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(SERV_PORT);

    // 绑定服务端地址信息
    bind(listenFd, (SA *) &serverAddress, sizeof(serverAddress));

    // 监听服务端地址
    listen(listenFd, LISTENQ);

    maxFd = listenFd;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; ++i) {
        client[i] = -1;
    }

    FD_ZERO(&allSet);
    FD_SET(listenFd, &allSet);

    for (;;) {
        readSet = allSet;
        readyNum = select(maxFd + 1, &readSet, NULL, NULL, NULL);

        if (FD_ISSET(listenFd, &readSet)) { // new client connect
            clientLength = sizeof(clientAddress);
            connectFd = accept(listenFd, (SA *) &clientAddress, &clientLength);
            printf("new client connected : %s:%d\n",
                   inet_ntop(AF_INET, &clientAddress.sin_addr, 4, NULL),
                   ntohs(clientAddress.sin_port));

            for (i = 0; i < FD_SETSIZE; ++i) {
                if (client[i] < 0) {
                    // 保存连接描述符
                    client[i] = connectFd;
                    break;
                }
            }

            // 此时已经超过1024个客户端，无法继续服务
            if (i == FD_SETSIZE) {
                perror("too many clients !");
                exit(1);
            }

            // 将连接描述符添加到所有集中
            FD_SET(connectFd, &allSet);

            // 更新最大描述符
            if (connectFd > maxFd) {
                maxFd = connectFd;
            }

            // 记录client[]中后加入的描述符烦人位置
            if (i > maxi) {
                maxi = i;
            }
            if (--readyNum <= 0) {
                continue;
            }
        }

        // 检查所有的客户端，准备数据
        for (i = 0; i <= maxi; ++i) {
            if ((socketFd = client[i]) < 0) {
                continue;
            }
            // 如果当前的客户端连接描述符已经就绪，处理即可
            if (FD_ISSET(socketFd, &readSet)) {
                if ((readNum == read(socketFd, buffer, MAXLINE)) == 0) {
                    // 关闭该连接
                    close(socketFd);
                    // 清除全集中的描述符信息
                    FD_CLR(socketFd, &allSet);
                    // 重置该位置为可用状态
                    client[i] = -1;
                } else if (readNum < 0) {
                    perror("connect error ");
                    exit(1);
                } else {
                    write(socketFd, buffer, readNum);
                }
                if (--readyNum <= 0) {
                    break;
                }
            }
        }
    }

}