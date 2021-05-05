#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[]){
    int sock;
    FILE *fp;

    char buf[BUF_SIZE];
    int read_cnt;
    struct sockaddr_in serv_addr;
    if(argc != 3) {
        puts("Please input IP and port");
        exit(1);
    }

    fp = fopen("receive.dat", "wb");  //创建新文件以保存服务器端传输的文件数据
    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while((read_cnt = read(sock, buf, BUF_SIZE)) != 0) {   //接收数据并保存到23行创建的文件中，直到收到EOF为止
        fwrite((void*)buf, 1, read_cnt, fp);
    }

    puts("Received file data");
    write(sock, "Thank you", 10); //向服务器端发送表示感谢的信息。若服务器端未关闭输入流，则可以接收该消息。
    fclose(fp);
    close(sock);
    return 0;
} 

void error_handling(char *message) {
    fputs(message,stderr);
    fputc('\n', stderr);
    exit(1);
}