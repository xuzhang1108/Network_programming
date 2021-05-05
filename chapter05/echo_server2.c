#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len;         //后面调用read函数时，返回值用它接收。

    if(argc != 2){
        printf("Please input port number\n");
        exit(1);
    }

    struct sockaddr_in serv_addr, clnt_addr;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);  //创建套接字
    if(serv_sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));   //初始化IPv4套接字地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)  //绑定本地地址到套接字
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)              //是套接字进入等待被连接状态
        error_handling("listen() error");

    socklen_t clnt_size = sizeof(clnt_addr);

    for(int i = 0; i < 5; i++) {
        if((clnt_sock =accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_size)) == -1)  //进入连接状态，从等待队列中取出连接请求
            error_handling("accept() error");
        else
            printf("Client %d connected\n", i+1);

        while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
        {
            write(clnt_sock, message, str_len);
        }
        close(clnt_sock);
    }
    
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message,stderr);
    fputc('\n', stderr);
    exit(1);
}