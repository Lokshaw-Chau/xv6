#include "kernel/types.h"
#include "user.h"
void sieve(int* parent_p);

int main(){
    int p[2];
    pipe(p);
    if(fork() == 0){
        close(p[1]);
        sieve(p);
        exit(0);
    }
    else{
        //parent
        close(p[0]);
        for(int i=2; i<=35; i++){
            write(p[1],&i,sizeof(int));
            //pipe is used as an array which contains all the numbers that pass to the next layer.
        }
        close(p[1]);
        //If parent exit while children are not, '$ ' will interleave in the output.
        wait(0);
        exit(0);
    }
}

void sieve(int* parent_p){
    //child
    int prime;
    int num;
    //finish when there is no number passing from the parent
    if(read(parent_p[0], &prime, sizeof(int))){
        int p_child[2];
        pipe(p_child);
        printf("prime %d\n",prime);
        if(fork()==0){
            //child
            close(p_child[1]);
            sieve(p_child);
            exit(0);
        }
        else{
            //parent
            close(p_child[0]);
            while (read(parent_p[0],&num,sizeof(int))){
                if(num % prime != 0){
                    write(p_child[1],&num,sizeof(int));
                }
            }
            close(parent_p[0]);
            close(p_child[1]);
            wait(0);
            exit(0);
        }
    }
    //make sure the final child can exit and close parent pipe
    close(parent_p[0]);
    exit(0);
}