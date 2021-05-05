#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024
#define OPSZ 4
#define RLT_SIZE 4
void error_handling(char *message);

int main(int argc, char *argv[]){
    int sock;
    int result, opnd_cnt, i;
    char opmsg[BUF_SIZE];
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
        error_handling("connect error");
    else
        printf("Connected..........\n");

   
    printf("Operand count: ");
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;

    for(i = 0; i < opnd_cnt; i++){
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&opmsg[OPSZ*i + 1]);
    }
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[OPSZ*opnd_cnt + 1]);

    write(sock, opmsg, OPSZ*opnd_cnt+2);

    read(sock, &result, RLT_SIZE);
    printf("Operation result: %d\n", result);
        
    
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}