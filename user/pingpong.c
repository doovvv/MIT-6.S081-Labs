#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
    int p[2];
    if (pipe(p) == -1) {
        printf("pipe failed");
        exit(1);
    }
    int pid = fork();
    if (pid == -1) {
        printf("fork failed");
        exit(1);
    }
    if(pid == 0){ //child process
        char buf[1];
        read(p[0],buf,1);
        printf("%d: received ping\n",getpid());
        write(p[1],buf,1);
        exit(0);
    }
    else{
        char a = 'a';
        write(p[1],&a,1);
        int staus();
        read(p[0],&a,1);
        printf("%d: received pong\n",getpid());
        int status;
        wait(&status);
        exit(0);
    }

}