#include "kernel/types.h"
#include "user.h"

int main(){
    int p1[2];
    int p2[2];
    int ret;
    char buffer1[64];
    char buffer2[64];
    pipe(p1);
    pipe(p2);
    ret = fork();
    if(ret == 0){
        //child
        close(p1[1]);
        close(p2[0]);
        read(p1[0],buffer2,64);
        printf("%d: received %s\n",getpid(),buffer2);
        write(p2[1],"pong",64);
        close(p2[1]);
        close(p1[0]);
        exit(0);
    }
    else{
        //parent
        close(p1[0]);
        close(p2[1]);
        write(p1[1],"ping",64);
        read(p2[0],buffer1,64);
        printf("%d: received %s\n",getpid(),buffer1);
        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
}