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
str_cli(FILE *fp, int sockfd) {
    int maxfdp1;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];

    for (;;) {
        FD_ZERO(&rset);
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {    /* socket is readable */
            int readNum;
            if ((readNum = read(sockfd, recvline, MAXLINE)) == 0) {
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

        if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
            if (fgets(sendline, MAXLINE, fp) == NULL)
                return;        /* all done */
            write(sockfd, sendline, strlen(sendline));
        }
    }
}

int
main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        perror("usage: tcpcli <IPaddress>");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);        /* do it all */

    exit(0);
}