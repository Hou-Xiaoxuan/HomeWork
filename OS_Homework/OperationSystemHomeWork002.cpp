/*
内容：操作系统第二次上机代码，进程相关
系统：manjaro KDE
时间：2021/05/18
笔记：1. pause()函数：pause()函数和system("pause")的功能并不相同。
     pause()的作用是将进程休眠，收到信号导致进程被终止或被信号处理函数处理，
     对于后者，如果信号处理函数不退出进程，在调用信号处理函数之后，pause() 将返回，继续执行其后的程序。
     2. kill()函数的作用并不只是”kill“，它是用来在进程间发送信号的。
*/
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<wait.h>
#include<iostream>
#include<stdlib.h>
#include<stdlib.h>
#include<string>
using namespace std;

void sigcat()
{
    cout<<"Process Continue......"<<endl;
}

int main()
{
    freopen("out.txt","aw",stdout);
    int pid_ls, pid_ps;
    int status_ls, status_ps;
    signal(SIGINT, (sighandler_t)sigcat);
    cout<<"Pid: "<<getpid()<<"--创建执行ls的子进程，子进程pid为 "<<pid_ls<<endl;
    pid_ls = fork();
    if(pid_ls==0) // 执行ls命令的子进程执行的命令
    {
        cout<<"Pid: "<<getpid()<<"--执行ls命令的进程已经创建"<<endl;
        // pause(); // 等待唤醒
        cout<<"Pid: "<<getpid()<<"-----------------执行l\"s -l\"，输出如下----------------"<<endl;
        system("ls -l >> out.txt");
        cout<<"Pid: "<<getpid()<<"------------------------------------------------------"<<endl;
    }
    else
    {
        cout<<"Pid: "<<getpid()<<"--创建执行ps命令的进程，子进程pid为 "<<pid_ps<<endl;
        pid_ps = fork();
        if(pid_ps==0)// 执行ps命令的程序执行的命令
        {
            cout<<"Pid: "<<getpid()<<"--执行ps命令的进程已经创建"<<endl;
            pause(); // 等待唤醒
            cout<<"Pid: "<<getpid()<<"-----------------执行\"ps\"，输出如下---------------"<<endl;
            system("ps >> out.txt");
            cout<<"Pid: "<<getpid()<<"--------------------------------------------------"<<endl;
        }
        else // 主程序执行的命令
        {
            cout<<"Pid: "<<getpid()<<"--首先唤醒进程"<<pid_ls<<"执行ls -l命令"<<endl;
            kill(pid_ls, SIGINT);
            waitpid(pid_ls, &status_ls, 0);
            cout<<"Pid: "<<getpid()<<"--进程 Pid"<<pid_ls<<"执行完毕"<<endl;
            cout<<"Pid: "<<getpid()<<"--然后唤醒进程"<<pid_ps<<"执行ps命令"<<endl;
            kill(pid_ps, SIGINT);
            waitpid(pid_ps, &status_ps, 0);
            cout<<"Pid: "<<getpid()<<"--进程 Pid"<<pid_ps<<"执行完毕"<<endl;
        }
    }
    cout<<"Pid"<<getpid()<<"--本进程执行结束！"<<endl;
    return 0;
}

