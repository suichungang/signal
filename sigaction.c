#include <signal.h>
#include <stdio.h>
#include <unistd.h>


void sinal_handler(int sig)
{
    printf("oh, got a signal %d\n", sig);

    int i = 0;
    for (i = 0; i < 5; i++)
    {
        printf("signal func %d, signal %d\n", i, sig);
        sleep(1);
    }
}

int main(int argc, char** argv)
{
    struct sigaction act;
    act.sa_handler = sinal_handler;
    // 清空信号屏蔽集
    sigemptyset(&act.sa_mask);
    // 屏蔽信号
    // sigaddset(&act.sa_mask, SIGQUIT);
    sigaddset(&act.sa_mask, SIGTSTP);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

    struct sigaction act_2;
    act_2.sa_handler = sinal_handler;
    sigemptyset(&act_2.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGQUIT, &act_2, NULL);

    while(1)
    {
        sleep(1);
    }

    return 0;
}


