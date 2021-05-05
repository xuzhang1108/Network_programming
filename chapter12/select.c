#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads);   //0是标准输入

    /*
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;
    */

   while(1) {
       //temps = reads;
       timeout.tv_sec = 5;       //之所以不用上面的的初始化timeval的成员因为，select函数调用后都是使用超市前剩余的值。
       timeout.tv_usec = 0;
       result = select(1, &reads, 0, 0, &timeout);
       if(result == -1) {
           puts("select() error!");
           break;
       }
       else if(result == 0) {
           puts("Time-out!");
       }
       else {
           printf("Result = %d\n", result);
           if(FD_ISSET(0, &reads)) {
               str_len = read(0, buf, BUF_SIZE);
               buf[str_len] = 0;
               printf("message from console: %s", buf);
           }
       }
   }
   return 0;
}