#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();

    if(pid == 0) {
        puts("Hi, I am child");
    }
    else {
        printf("Child Process ID: %d \n", pid);
        //sleep(30);
    }

    if(pid == 0) {
        puts("End child process");
    }
    else {
        puts("End parent process");
    }
    return 0;
}