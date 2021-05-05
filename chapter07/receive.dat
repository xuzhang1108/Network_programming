#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t adr_sz;

    if(argc != 2) {
        puts("Please input port!");
        exit(1);
    }

    fp = fopen("file_server.c", "rb"); //打开文件向客户端传输服务器的源文件file_server.c
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    adr_sz = sizeof(clnt_addr);
    if((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz)) == -1)
        error_handling("accept() error");
    
    while(1) {          //向客户端传输文件数据而编写的循环语句, 此客户端在40行代码accept()函数处被连接
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if(read_cnt < BUF_SIZE) {
            write(clnt_sock, buf, read_cnt);
            break;
        }
        write(clnt_sock, buf, BUF_SIZE);
    }

    shutdown(clnt_sock, SHUT_WR);  //发送文件后针对输出流进行半关闭。这样就像客户端传输了EOF，同时客户端也知道文件传输完成。
    read(clnt_sock, buf, BUF_SIZE);  //只关闭了输出流，输入流还存在故可以读取客户端数据。
    printf("Message from client: %s \n", buf);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}


void error_handling(char *message) {
    fputs(message,stderr);
    fputc('\n', stderr);
    exit(1);
}