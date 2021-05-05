#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50

void error_handling(char *message);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t adr_sz;
    int str_len;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;    //定义事件数组
    struct epoll_event event;      //定义要监控的事件
    int epfd, event_cnt;             

    if(argc != 2) {
        puts("Please input port!");
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    /* 调用epoll_create将创建一个以epfd为根节点的红黑树（平衡二叉树）*/
    epfd = epoll_create(EPOLL_SIZE);    
    ep_events = malloc(sizeof(struct epoll_event) *EPOLL_SIZE);

    event.events = EPOLLIN;
    event.data.fd = serv_sock;

    /* EPOLL_CTL_ADD/DEL/ERR,在以epfd为根节点的红黑树中添加/删除/报错对应事件。*/
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);  

    while(1) {
        /* epoll_wait为服务器阻塞监听事件， ep_events为struct epoll_event类型的数组，
           EPOLL_SIZE定义数组大小， -1永久阻塞 */
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);  
        if(event_cnt == -1) {
            puts("epoll_wait() error");
            break;
        }

        for(int i = 0; i < event_cnt; i++) {
            if(ep_events[i].data.fd == serv_sock) {  //判断满足的事件是不是serv_sock,如果是接收请求
                adr_sz = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz); //接收请求，建立连接
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);  //将连接创建的clnt_sock套接字放入以epfd为根节点的红黑树里
                printf("connected client: %d \n", clnt_sock);
            }
            else {   //如果不是serv_sock套接字，那就是clnt_sock.
                str_len = read(ep_events[1].data.fd, buf, BUF_SIZE);  //读取客户端发送的消息
                if(str_len == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("close client: %d \n", ep_events[i].data.fd);
                }
                else {            //回射操作
                    write(ep_events[i].data.fd, buf, str_len);
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