#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>

#define BUF_SIZE 1024

#define SERVER_PORT 9002
#define SERVER_ADDR "127.0.0.1"
#define FILE_PATH "../test.txt"
char buf[BUF_SIZE];

int main()
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT); // или любой другой порт...
    addr.sin_addr.s_addr = inet_addr(SERVER_ADDR); // htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    FILE *in = fopen(FILE_PATH,"rb");   // обработать !!!

    while(!feof(in)) {
        unsigned long number_of_bytes = fread(buf,1, sizeof(buf), in);
        if(number_of_bytes != 0) {
            send(sock, buf, number_of_bytes,0);
            recv(sock, buf, number_of_bytes, 0);
            printf("%s", buf);
        }
    }
    fclose(in);

    //printf("%s", buf);
    close(sock);

    return 0;
}