#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#define BUF_SIZE 4
#define EVENTS_SIZE 50

void error_handling(char * message);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    int str_len;
    int epfd, event_cnt;
    char buf[BUF_SIZE];
    socklen_t adr_sz;
    struct sockaddr_in serv_addr, clnt_addr;
    struct epoll_event event;
    struct epoll_event *events;

    if(argc != 2) {
        printf("Please input port!\n");
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error!");
    
    epfd = epoll_create(EVENTS_SIZE);
    events = malloc(sizeof(struct epoll_event)*EVENTS_SIZE);
    
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);
    
    while(1) {
        event_cnt = epoll_wait(epfd, events, EVENTS_SIZE, -1);
        if(event_cnt == -1) {
            printf("epoll_wait error()!\n");
            break;
        }
        
        puts("return epoll_wait()");
        for(int i = 0; i < event_cnt; i++) {
            if(events[i].data.fd == serv_sock) {
                adr_sz = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
                if(clnt_sock == -1)
                    error_handling("accept() error!");
                
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d \n", clnt_sock);
            }
            else {
                str_len = read(events[i].data.fd, buf, BUF_SIZE);
                if(str_len == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                    printf("Client %d close\n", events[i].data.fd);
                }
                else {
                    write(events[i].data.fd, buf, str_len);
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}