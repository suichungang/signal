#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// 信号处理函数
void sinal_handler(int sig)
{
    printf("I got signal %d\n", sig);
    
    // 注册默认处理
    signal(SIGINT, SIG_DFL);
}

int main(int argc, char** argv)
{
    // 注册信号
    signal(SIGINT, sinal_handler);

    while(1)
    {
        sleep(1);
    }
}


