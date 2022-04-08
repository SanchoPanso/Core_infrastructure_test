#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>

#define BUF_SIZE 1024
#define SAVE_DIR ".."
#define FILE_NAME "test.txt"

#define SERVER_PORT 9002
#define SERVER_ADDR "127.0.0.1"

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[BUF_SIZE];
    long bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);   //htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);

    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

        FILE *out = fopen(SAVE_DIR "/" FILE_NAME, "wb");
        while(1)
        {
            bytes_read = recv(sock, buf, BUF_SIZE, 0);
            if(bytes_read <= 0) break;
            fwrite(buf, 1, bytes_read, out);
            send(sock, buf, bytes_read, 0);
        }
        fclose(out);
        close(sock);
        close(listener);
        break;
    }

    return 0;
}