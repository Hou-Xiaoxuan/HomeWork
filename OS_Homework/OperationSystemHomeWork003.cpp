/*
内容：操作系统第三次上机代码，进程相关
系统：manjaro KDE
时间：2021/05/25
笔记：
* pipe创建出的管道只能在父子进程之间通信，并且是严格的0读1写。
*/
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>
#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <string>
#include <cmath>
using namespace std;

// 总共要有三个进程，分别计算不同的东西；
int ct(int *pid)
{
    int cnt = 0;
    for (int i = 1; i <= 3; i++)
        if (cnt == 1)
            cnt++;
    return cnt;
}
int main()
{
    int a, b; // 需要计算的两个函数变量
    cout << "请输入a和b的值..." << endl;
    cin >> a >> b;

    int pid[4] = {0};  //
    int allpipe[4][2]; // 创建3个管道。1: 2->0 2: 3->0 3: 0->1 主进程使用0读，1写
    for (size_t i = 1; i <= 3; i++)
        pipe(allpipe[i]);
    pid[1] = fork();
    if (pid[1] == 0) // 计算f(x, y)的进程
    {
        int x, y;
        close(allpipe[1][1]);
        read(allpipe[1][0], &x, sizeof(int));
        read(allpipe[1][0], &y, sizeof(int));
        cout << "进程1计算出f(x,y)=" << x + y << endl;
    }
    else
    {
        pid[2] = fork();
        if (pid[2] == 0) // 计算f(x)的进程
        {
            close(allpipe[2][0]);
            int fx = 1;
            for (int i = 2; i <= a; i++)
                fx *= i;
            write(allpipe[2][1], &fx, sizeof(int));
            close(allpipe[2][1]);
            cout<<"进程2计算出f(x)="<<fx<<endl;
        }
        else
        {
            pid[3] = fork();
            if (pid[3] == 0) // 计算f(y)的进程
            {
                close(allpipe[3][0]);
                int fy = pow((1.0 + sqrt(5)) / 2, b) / sqrt(5.0) - pow((1.0 - sqrt(5)) / 2, b) / sqrt(5.0);
                write(allpipe[3][1], &fy, sizeof(int));
                close(allpipe[3][1]);
                cout<<"进程3计算出f(x)="<<fy<<endl;
            }
            else // 主进程
            {
                // 从进程2读一个数据
                close(allpipe[2][1]);
                int x;
                read(allpipe[2][0], &x, sizeof(int));
                close(allpipe[2][0]);
                // 从进程3读一个数据
                close(allpipe[3][1]);
                int y;
                read(allpipe[3][0], &y, sizeof(int));
                close(allpipe[3][0]);

                // 将x与y写给进程1
                close(allpipe[1][0]);
                write(allpipe[1][1], &x, sizeof(int));
                write(allpipe[1][1], &y, sizeof(int));
                close(allpipe[1][1]);
            }
        }
    }
    return 0;
}
