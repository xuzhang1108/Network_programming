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
    int str_len, recv_len, recv_cnt;
    char message[BUF_SIZE];

    struct sockaddr_in serv_addr;

    if(argc != 3){
        printf("Please input IP and port\n");
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else
        printf("Connected..............\n");

    while (1)
    {
        fputs("Input message (Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        str_len = write(sock, message, strlen(message));

        recv_len = 0;
        while (recv_len < str_len)
        {
            recv_cnt = read(sock, message, BUF_SIZE);
            if(recv_cnt == -1)
                error_handling("read() error");
            recv_len += recv_cnt;
        }
        printf("Message from server: %s \n", message);
    }
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}