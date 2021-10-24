/*
* Function: 第四次上机，进程的优先级调度
* Time: 2021/06/01
* Copyright: Lx
*/

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
using namespace std;
/*
kill pid * 来传递信号
 2: SIGINT
20: SIGTSTP
*/
void sigintcat()
{
    // 处理信号SIGINT，优先级+1
    int priority_now = getpriority(PRIO_PROCESS, getpid()); // 得到当前的优先级
    setpriority(PRIO_PROCESS, 0, priority_now + 1);
    cout<<"PID "<<getpid()<<": priority add, equal to "<<priority_now+1<<endl;
}
void sigtstpcat()
{
    // 处理SIGTSTP，优先级-1
    int priority_now = getpriority(PRIO_PROCESS, getpid()); // 得到当前的优先级
    setpriority(PRIO_PROCESS, 0, priority_now - 1);
    cout<<"PID "<<getpid()<<": priority minus, equal to "<<priority_now-1<<endl;
}

int main()
{
    signal(SIGINT, (sighandler_t)sigintcat);
    signal(SIGTSTP, (sighandler_t)sigtstpcat);
    int childpid = fork();
    if(childpid > 0) // 父进程
    {
        for(int i=1; i<=10; i++)
        {
            cout<<"Father"<<getpid()<<": i="<<i<<", priority is "<<getpriority(PRIO_PROCESS, 0)<<endl;
            sleep(7);
        }
    }
    else // 子进程
    {
        for(int i=1; i<=10; i++)
        {
            cout<<"Child"<<getpid()<<": i="<<i<<", priority is "<<getpriority(PRIO_PROCESS, 0)<<endl;
            sleep(7); 
        }
    }
    cout<<"PID "<<getpid()<<": process end."<<endl;
}
