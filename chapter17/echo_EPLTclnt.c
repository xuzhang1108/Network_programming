#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]) {
    int sock;
    int str_len;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_addr;

    if(argc != 3) {
        printf("Please input IP and port!\n");
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    else
        printf("Connected......\n");

    while(1) {
        fputs("Input message (Q to quit):", stdout);
        fgets(buf, BUF_SIZE, stdin);

        if(!strcmp(buf, "Q\n") || !strcmp(buf, "q\n"))
            break;
        write(sock, buf, strlen(buf));
        sleep(1);
        str_len = read(sock, buf, BUF_SIZE-1);
        buf[str_len] = 0;
        printf("Message from server: %s  size = %d\n", buf, str_len);
    }
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}