#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path){
    char *p;
    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p;
}

int find(char *path,char *target){
    int fd;
    int findFlag = 0;
    char buf[512],*p;
    struct dirent de;
    struct stat st;
    
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path); 
        return findFlag;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return findFlag;
    }
    switch (st.type){
    case T_FILE:
        if(strcmp(fmtname(path),target)==0){
            printf("%s\n",path);
            findFlag = 1;
        }
        break;
    case T_DIR:
        //TODO: ?????how to find in deep hierachy????
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf,path);
        p = buf+strlen(buf);
        *p++='/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            //'.' and '..' are included in every dictionary, skip it.
            if(strcmp(".",de.name) == 0||strcmp("..",de.name) == 0)
                continue;
            memmove(p,de.name,DIRSIZ);
            if(stat(buf, &st) < 0){
                continue;
            }
            if(find(buf,target) == 1){
                findFlag = 1;
            }
        }
        break;
    }
    close(fd);
    return findFlag;
}

int main(int argc, char* agrv[]){
    if ( argc != 3){
        printf("there should be 2 arguement while recieved %d",argc-1);
        exit(-1);
    }
    else{
        if(find(agrv[1],agrv[2]) == 0){
            printf("File not found!\n");
        }
        exit(0);
    }
}