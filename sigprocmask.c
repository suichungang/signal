#include <signal.h>
#include <stdio.h>
#include <unistd.h>
 
static void sig_quit(int);
 
int main (void) {
    sigset_t new_sig;
    sigset_t old_sig;
    
    signal(SIGQUIT, sig_quit);
 
    sigemptyset(&new_sig);
    sigaddset(&new_sig, SIGQUIT);
    sigprocmask(SIG_BLOCK, &new_sig, &old_sig); // 屏蔽信号
    printf("SIGQUIT blocked\n");
 
    sleep(5);
 
    printf("SIGQUIT unblocked\n");
    sigprocmask(SIG_SETMASK, &old_sig, NULL); // 解除屏蔽
 
    sleep(50);
    return 1;
}
 
static void sig_quit(int signo) {
    printf("catch SIGQUIT\n");
    signal(SIGQUIT, SIG_DFL);
}
