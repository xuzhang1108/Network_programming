#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024
#define OPSZ 4

void error_handling(char *message);
int calculate(int opnum, int opnds[], char oprator);

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    char opinfo[BUF_SIZE];
    int result, opnd_cnt;
    int recv_len, recv_cnt;
    struct sockaddr_in serv_addr, clnt_addr;

    if(argc != 2){
        printf("Please input port\n");
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    socklen_t clnt_size = sizeof(clnt_addr);
    for (int i = 0; i < 5; i++)
    {
        opnd_cnt = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_size);
        if(clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Client %d connected\n", i+1);
        
        read(clnt_sock, &opnd_cnt, 1);   //读取要做几个数的运算

        recv_len = 0;
        while (opnd_cnt*OPSZ+1 > recv_len)    //读取数据放入opinfo中
        {
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE);
            recv_len += recv_cnt;
        }

        //计算读取到的数据
        result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len-1]);
        write(clnt_sock, (char*)&result, sizeof(result));  //将计算求得的结果，传给客户端
        close(clnt_sock);   
    }
    close(serv_sock);
    return 0;
}

int calculate(int opnum, int opnds[], char op) {
    int result = opnds[0], i;
    switch (op)
    {
    case '+':
        for(i = 1; i < opnum; i++)
            result += opnds[i];
        break;
    case '-':
        for(i = 1; i < opnum; i++)
            result -= opnds[i];
        break;
    case '*':
       for(i = 1; i < opnum; i++)
            result *= opnds[i];
        break;
    }
    return result;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}