#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void timeout(int sig){
    if(sig ==  SIGALRM)
        puts("Timet out");
    alarm(2);
}

void keycontrol(int sig){
    if(sig == SIGINT)
        printf("CTRL+C pressed\n");
}

int main(int argc, char *argv[]){
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for (i = 0; i < 3; i++)
    {
        printf("wait....\n");
        sleep(100);
    }
    return 0;
    
}