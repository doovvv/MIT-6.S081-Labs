#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    if (pipe(p) == -1)
    {
        printf("pipe failed");
        exit(1);
    }
    int pid = fork();
    if (pid == -1)
    {
        printf("fork failed");
        exit(1);
    }
    if (pid != 0)
    {
        close(p[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        int status;
        wait(&status);
    }
    while (pid == 0)
    {
        close(p[1]);
        int prime = 0;
        read(p[0], &prime, sizeof(int));
        if (prime == 0)
        {
            exit(0);
        }
        printf("prime %d\n", prime);
        int number[35] = {0};
        int read_num = 1;
        int j = 0;
        while (read_num)
        {
            read_num = read(p[0], number + j, sizeof(int));
            j++;
        }
        // printf("%d", number[0]);
        close(p[0]);
        if (pipe(p) == -1)
        {
            printf("pipe failed");
            exit(0);
        }
        pid = fork();
        if (pid > 0)
        {
            close(p[0]);
            for (int i = 0; i < j; i++)
            {
                if (number[i] % prime != 0)
                {
                    write(p[1], number + i, sizeof(int));
                }
            }
            close(p[1]);
            int status;
            wait(&status);
            exit(0);
        }
    }
    exit(0);
}