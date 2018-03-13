#include    "unp.h"
#include    <time.h>

int
main(int argc, char **argv) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;

    /**
     * servaddr.sin_addr; // 32-bit IPv4 address in network
     * servaddr.sin_family;//AF_INET(IPV4)
     * servaddr.sin_port; // 16 bit TCP or UDP port
     * servaddr.sin_zero; // unused
     */

    char buff[MAXLINE];
    time_t ticks;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);    /* daytime server */

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    for (;;) {
        connfd = Accept(listenfd, (SA *) NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

        Close(connfd);
    }
}

