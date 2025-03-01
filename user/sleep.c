#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
    if(argc != 2){
        printf("the num of argument is incorrect\n");
    }
    int sleep_time = atoi(argv[1]);
    sleep(sleep_time);
    exit(0);
}