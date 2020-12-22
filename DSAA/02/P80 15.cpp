#include<iostream>
#include<queue>
using namespace std;

/*
*题目要求：实现一个坏心队列，用Pfront和Prear最为队头指针和队尾指针，tag表示队列是空还是不空，要求设计相关运算算法
*这题的要求相对不精确，因此使用类模板来实现
*使用数组实现队列
*相关算法 暂定： push,front,pop,size,empty
*/
#define debug cout
template<class T>
class My_queue
{
    const int inf=0x3f3f3f3f;
    T* array;
    int Pfront, Prear;
    bool tag;//是否为满的标志
    unsigned int Size;//队列当前以用容量
    unsigned int Capcity;//队列容量
public:
    My_queue(unsigned int cap=5)
    {
        Capcity=cap;
        array = new T[Capcity];
        Size=0;
        tag=false;
        Pfront=Prear=0;
    }
    void push(T value)
    {
        if(tag==true){//队列已满
            debug<<"the queue if full, can't push "<<value<<"!"<<endl;
            return;
        }
        Size++;
        array[Prear]=value;
        Prear=(Prear+1)%Capcity;
        // if(Size==Capcity) tag=true;    
        if(Pfront==Prear) tag=true;/*使用Pfront==Prear来作为队满的条件*/
    }
    T front()
    {
        if(empty()) return inf;
        return array[Pfront];
    }
    void pop()
    {
        if(empty()){
            debug<<"the queue is empty, cant pop"<<endl;
            return;
        }
        tag=false;//弹出后肯定由空位了
        Pfront=(Pfront+1)%Capcity;
        Size--;
    }
    bool empty()
    {
        // if(Size==0) return true;
        if(tag==false and Prear==Pfront) return true;/*使用Pfront和Prear判断是否为空*/
        return false;
    }
    unsigned int size()
    {
        return Size;
    }
    ~My_queue()
    {
        delete[] array;
    }
};

int main()
{
    #ifdef _DEBU
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    // 测试：
    My_queue<char> que(5);
    for(int i=0;i<=9;i++)
    {
        if(!(i&1)) que.pop();
        que.push(i+'0');
    }
    while(!que.empty()){
        cout<<que.front()<<" ";
        que.pop();
    }
    cout<<endl;

    return 0;
}