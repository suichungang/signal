#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// fork系统调用之后，父子进程将交替执行，执行顺序不定。
//
// 如果父进程先退出，子进程还没退出那么子进程的父进程将变为init
// 进程（托孤给了init进程）。（注：任何一个进程都必须有父进程）
//
// 如果子进程先退出，父进程还没退出，那么子进程必须等到父进程
// 捕获到了子进程的退出状态才真正结束，否则这个时候子进程就成为
// 僵进程（僵尸进程：只保留一些退出信息供父进程查询）



// 信号处理函数
void sinal_handler(int sig)
{
    printf("I got signal %d, pid:%d \n", sig, getpid());
}

int main(void)
{
    int status = 0;
    pid_t pid;
    printf("before calling fork,calling process pid = %d\n",getpid());

    // 注册信号
    signal(SIGCHLD, sinal_handler);

    // 创建进程
    pid = fork();
    if(pid == -1)
    {
        printf("fork error\n");
        return 1;
    }

    // 子进程
    if(pid == 0){
        printf("this is child process and child's pid = %d,parent's pid = %d\n", getpid(), getppid());
        sleep(5);
    }
    // 父进程
    else
    {
        printf("this is parent process and pid =%d ,child's pid = %d\n", getpid(), pid);

        /*
           进程一旦调用了wait()， 就立即阻塞自己，由wait自动分析是否有当前进程的某个子进程已经退出，
           如果让它找到了一个已经变成僵尸的子进程，wait就会收集这个子进程的信息，并把它彻底销毁后
           返回；如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到一个子进程结束或者该进程
           接到了一个指定的信号为止。
           */
        // wait(&status);

        /*
           waitpid()的作用和wait()一样，但它并不一定要等待第一个终止的子进程，它还有若干选项，如可
           提供一个非阻塞版本的wait()功能等。实际上wait()函数只是waitpid()函数的一个特例，在Linux
           内部实现wait()函数时直接调用的就是waitpid()函数。
           */
        waitpid(pid, NULL, WUNTRACED | WCONTINUED);

         while(1)
         {
             sleep(1);
         }
    }

    return 0;
}
