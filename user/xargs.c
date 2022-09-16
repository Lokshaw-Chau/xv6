#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


int main(int argc, char* argv[]){
    char *newArgv[MAXARG];
    int i;
    for (i=1; i<argc; i++){
        newArgv[i-1]=argv[i];
    }       
    char buffer[64];
    int orginArgc = i-1;
    char arg[64];
    char* split = arg;
    //use while to assure read all the output from the pipe
    //the output won't be split by '\n'
    while (read(0,buffer,sizeof(buffer))>0){
        int p = 0;
        int j = 0;
        int q = orginArgc;
        while(buffer[j]!='\0'){
            if (buffer[j]==' '){
                //in terms that there may be many blank in one line of the output
                if(buffer[j+1]!=' '&&buffer[j+1]!='\n'){
                    //newArgv[q] is actually a pointer
                    //It points to every begining address of the parameter in arg, and it will end at '\0' itself.
                    newArgv[q] = split;
                    i++;
                    //to memmorize all the argument, you should put them in a sequential char array, seperated by'\0'
                    arg[p]='\0';
                    split = &arg[p+1];
                    p++;
                }
            }
            //run command for each line of the output
            else if (buffer[j]=='\n')
            {   
                arg[p]='\0';
                newArgv[q] = split;
                newArgv[q+1]=0;
                if(fork()==0){
                    exec(newArgv[0],newArgv);
                    exit(0);
                }   
                wait(0);
                split = &arg[p+1];
                q = orginArgc;
            }
            else{
                arg[p] = buffer[j];
                p++;
            }
            j++;
        }
    }
    exit(0);
}