//
// Created by star on 18-3-13.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>


#define MAX_LISTEN 5
#define PORT 1987
#define IP "127.0.0.1"

int
main(int argc, char *argv[]) {
    int connectFileDescriptor;
    // socket文件描述符
    int socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    // 检查socket是否创建成功
    if (socketFileDescriptor < 0) {
        perror("create socket failed ! ");
        exit(1);
    }

    struct sockaddr_in addrClient;
    int clientSize = sizeof(struct sockaddr_in);

    struct sockaddr_in addrServer;
    memset(&addrServer, 0, sizeof(addrServer));

    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(PORT);
    addrServer.sin_addr.s_addr = inet_addr(IP);

    // 绑定端口号
    if (bind(socketFileDescriptor, (struct sockaddr *) &addrServer, sizeof(struct sockaddr_in)) < 0) {
        perror("bind error");
        exit(1);
    }

    int receiveNum;
    int sendNum;
    char receiveBuffer[100];
    char sendBuffer[100];

    // 用一个数组记录描述符的状态
    int ready, maxFileDescriptor;
    int client[FD_SETSIZE];
    for (int i = 0; i < FD_SETSIZE; ++i) {
        client[i] = -1;
    }

    fd_set readSet;
    maxFileDescriptor = socketFileDescriptor;

    // 最大可用描述符的个数受到操作系统内核的影响，这里是1024
    printf("max file descriptor num : %d\n", FD_SETSIZE);

    while (1) {
        // 重置监听的描述符
        FD_ZERO(&readSet);
        FD_SET(socketFileDescriptor, &readSet);
        // 遍历
        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (client[i] == 1) {
                FD_SET(i, &readSet);
            }
        }

        // 开始监听描述符，是异步的，不会阻塞
        ready = select(maxFileDescriptor + 1, &readSet, NULL, NULL, NULL);

        // 可用描述符如果是创建连接描述符的，则创建一个连接
        if (FD_ISSET(socketFileDescriptor, &readSet)) {
            connectFileDescriptor = accept(socketFileDescriptor,
                                           (struct sockaddr *) &addrClient,
                                           &clientSize);
            if (connectFileDescriptor < 0) {
                perror("accept failed !");
                exit(1);
            }

            FD_SET(connectFileDescriptor, &readSet);
            FD_CLR(socketFileDescriptor, &readSet);

            if (connectFileDescriptor > maxFileDescriptor) {
                maxFileDescriptor = connectFileDescriptor;
            }
            client[connectFileDescriptor] = 1;
        }

        // 检查所有描述符，查看可读的是 哪个，对他进行IO读写
        for (int j = 0; j < FD_SETSIZE; ++j) {
            if (FD_ISSET(j, &readSet)) {
                receiveNum = recv(j, receiveBuffer, sizeof(receiveBuffer), 0);
                if (receiveNum <= 0) {
                    FD_CLR(j, &readSet);
                    client[j] = -1;
                }
                receiveBuffer[receiveNum] = '\0';
                // 初始化发送缓冲区
                memset(sendBuffer, 0, sizeof(sendBuffer));
                // 填充发送缓冲区
                sprintf(sendBuffer, "Server proc got %d bytes\n", receiveNum);
                // 发送数据
                sendNum = send(j, sendBuffer, sizeof(sendBuffer), 0);
                if (sendNum <= 0) {
                    FD_CLR(j, &readSet);
                    client[j] = -1;
                }
            }
        }
    }

    return 0;
}
