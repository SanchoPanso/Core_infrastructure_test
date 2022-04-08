#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>

#define BUF_SIZE 1024

#define SERVER_PORT 9002
#define SERVER_ADDR "127.0.0.1"
#define FILE_PATH "../test.txt"
#define SAVED_FILE_NAME "change.txt"
char buf[BUF_SIZE];

void send_message(int sock) {

    // send filename
    strcpy(buf, SAVED_FILE_NAME);
    send(sock, buf, BUF_SIZE, 0);
    // printf("%s", buf);

    // send file data
    FILE *in = fopen(FILE_PATH,"rb");
    if (in == NULL) {
        perror("file");
        exit(1);
    }

    while(!feof(in)) {
        unsigned long number_of_bytes = fread(buf,1, sizeof(buf), in);
        if(number_of_bytes != 0) {
            send(sock, buf, number_of_bytes,0);
            // recv(sock, buf, number_of_bytes, 0);
            // printf("%s", buf);
        }
    }
    fclose(in);
}

int main()
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    struct in_addr in_p;
    if (inet_aton(SERVER_ADDR, &in_p) == 0) {
        perror("address");
        exit(3);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = in_p.s_addr; // inet_addr(SERVER_ADDR);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    send_message(sock);

    //printf("%s", buf);
    close(sock);

    return 0;
}