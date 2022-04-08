#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/unistd.h>

#define BUF_SIZE 1024
#define SAVE_DIR "../"
#define FILE_NAME "test.txt"

#define SERVER_PORT 9002
#define SERVER_ADDR "127.0.0.1"

char buf[BUF_SIZE];

void process_message(int sock) {

    long bytes_read = 0;
    printf("Start processing received message...\n");

    // receive filename
    char filename[BUF_SIZE];
    bytes_read = recv(sock, filename, BUF_SIZE, 0);
    if (bytes_read <= 0)
        return;
    char filepath[BUF_SIZE] = SAVE_DIR;
    strcat(filepath, filename);
    printf("File will be saved in \"%s\"\n", filepath);

    // receive file data
    FILE *out = fopen(filepath, "wb");
    if (out == NULL) {
        perror("file");
        exit(1);
    }

    while(true) {
        bytes_read = recv(sock, buf, BUF_SIZE, 0);
        if (bytes_read <= 0) break;
        fwrite(buf, 1, bytes_read, out);
        //send(sock, buf, bytes_read, 0);
    }
    fclose(out);
    printf("File saved\n");
}

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    long bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
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
    addr.sin_addr.s_addr = in_p.s_addr;//inet_addr(SERVER_ADDR);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    printf("Listening...\n");
    listen(listener, 1);

    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0) {
            perror("accept");
            exit(3);
        }
        process_message(sock);
        close(sock);
        close(listener);
        break;
    }

    return 0;
}