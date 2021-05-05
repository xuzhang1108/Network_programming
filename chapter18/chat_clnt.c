#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void *send_msg(void *arg);
void *recv_msg(void *arg);
void error_handling(char *msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    pthread_t send_thread, recv_thread;
    void *thread_return;
    if(argc != 4) {
        printf("Please input IP, port and name!");
        exit(1);
    }

    sprintf(name, "[%s]", argv[3]);
    //printf("sprintf called\n");
    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    //printf("memset called\n");
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    
    pthread_create(&send_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&recv_thread, NULL, recv_msg, (void*)&sock);
    pthread_join(send_thread, &thread_return);
    pthread_join(recv_thread, &thread_return);
    close(sock);
    return 0;
}


void *send_msg(void *arg) {    //发送消息的线程
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE+BUF_SIZE];
    while(1) {
        fgets(msg, BUF_SIZE, stdin);
        if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void *recv_msg(void *arg) {     //接收消息的线程
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE+BUF_SIZE];
    int str_len;
    while(1) {
        str_len = read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
        if(str_len == -1)
            return (void*)-1;
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}

void error_handling(char *msg) {
    fputs(msg,stdout);
    fputc('\n', stdout);
    exit(1);
}