#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

// Daemon（守护进程）是运行在后台的一种特殊进程。它独立于控制终端并且周期性
// 地执行某种任务或等待处理某些发生的事件。它不需要用户输入就能运行而且提供
// 某种服务，不是对整个系统就是对某个用户程序提供服务。Linux系统的大多数服
// 务器就是通过守护进程实现的。常见的守护进程包括系统日志进程syslogd、 web
// 服务器httpd、邮件服务器sendmail和数据库服务器mysqld等。

// 进程组(process group)： 一个或多个进程的集合，每个进程都有一个进程组ID，
//      这个ID就是进程组长的进程ID
// 会话期(session)： 一个或多个进程组的集合，每个会话有唯一一个会话首进程
//      (session leader)，会话ID为会话首进程ID
// 控制终端(controlling terminal) ：每一个会话可以有一个单独的控制终端，
//       与控制终端连接的会话首进程就是控制进程(controlling process)。
//       这时候，与当前终端交互的就是前台进程组，其他的都是后台进程组。

#define ERR_EXIT(m) \
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
}\
while (0);\

void creat_daemon(void);

int main(int argc, char** argv)
{
    time_t t;
    int fd;

    creat_daemon();
    while(1){
        fd = open("daemon.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
        if(fd == -1)
            ERR_EXIT("open error");
        t = time(0);
        char *buf = asctime(localtime(&t));
        write(fd, buf, strlen(buf));
        close(fd);
        sleep(60);
    }

    return 0;
}

void creat_daemon(void)
{
    pid_t pid;
    pid = fork();
    if(pid == -1)
        ERR_EXIT("fork error");

    // 父进程退出
    if(pid > 0)
        exit(EXIT_SUCCESS);

    // 子进程设置进程组号
    if(setsid() == -1)
        ERR_EXIT("SETSID ERROR");

    // 再次创建子进程, 禁止进程重新打开控制终端
    pid = fork();
    if(pid > 0)
        exit(EXIT_SUCCESS);

    // 设置工作目录
    chdir("/Users/suichungang/Documents/project/linux");

    // 关闭描述符
    int i;
    for(i = 0; i < 3; ++i)
    {
        int fd;
        close(i);
        fd = open("/dev/null", O_RDWR);
        dup2(fd,i);
    }

    // 为进程设置文件模式创建屏蔽字，并返回以前的值。 mode & (~cmask)
    umask(0);

    return;
}
