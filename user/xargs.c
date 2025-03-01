#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
int main(int argc, char *argv[])
{
    // 记录用于下次换行之后继续使用
    int original_xargc = argc - 1;
    char *original_xargv[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        original_xargv[i - 1] = argv[i];
    }
    int xargc = 0;
    char *xargv[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        xargv[xargc] = argv[i];
        xargc++;
    }
    char buf[512];
    int i = 0;
    char *p = buf;
    while (read(0, buf + i, 1))
    {
        if (buf[i] == ' ')
        {
            // 此时遍历到一个参数,添0形成字符串
            buf[i] = '\0';
            // 放入参数列表中
            xargv[xargc] = p;
            xargc++;
            xargv[xargc] = 0; // 末尾添加 NULL
            // 更新p，用于找下一个参数
            p = buf + i + 1;
        }
        else if (buf[i] == '\n')
        {
            buf[i] = '\0';
            xargv[xargc] = p;
            xargc++;
            xargv[xargc] = 0; // 末尾添加 NULL
            // printf("%d\n", i);
            int pid = fork();
            if (pid == 0)
            {
                // 子进程去执行命令
                // printf("%s", xargv[0]);
                exec(xargv[0], xargv);
                exit(0);
            }
            else
            {
                int status;
                wait(&status);
                i = 0;
                p = buf;
                xargc = original_xargc;
                memcpy(xargv, original_xargv, sizeof(original_xargv));
            }
        }
        i++;
    }
    exit(0);
}