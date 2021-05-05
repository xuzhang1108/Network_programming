#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[]){
    int serv_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_addr_len;

    struct sockaddr_in serv_addr, clnt_addr;
    if(argc != 2){
        printf("Please input port");
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    while (1)
    {
        clnt_addr_len = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_len);

        sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_addr, clnt_addr_len);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message,stderr);
    fputc('\n', stderr);
    exit(1);
}