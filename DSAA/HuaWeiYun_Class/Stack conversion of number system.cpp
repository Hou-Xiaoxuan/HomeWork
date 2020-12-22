
/*课后习题1.8-g

		g.	设计算法把一个十进制整数转换为二至九进制之间的任意进制数输出。
			void Binaryconv(int n, int m)
*/
#include <iostream>
using namespace std;
class stack {
private:
    int maxsize;
    int top;
    int* st;

public:
    stack(int size)
    {
        maxsize = size;
        st = new int[maxsize];
        top = -1;
    }

    void push(int num)
    {
        st[++top] = num;
    }

    void pop()
    {
        int item;
        while (top != -1) {
            item = st[top--];
            cout << item;
        }
    }
};

/*
TODO:设计算法把一个十进制整数转换为二至九进制之间的任意进制数输出。
其中，n为要转换的10进制整数， m为进制，取值为2到9。
提示：利用栈解决
 */
void Binaryconv(int n, int m) //进制转换
{
    stack s(32);
    cout << "整数" << n << "转成" << m << "进制后的结果是:";
    /* 作业：*/
        while(n>=m){
            s.push(n%m);
            n/=m;
        }
        if(n) s.push(n);
    s.pop();
    cout<<endl;
}
int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    int n, m;
    while(cin>>n>>m)
        Binaryconv(n, m);
    return 0;
}