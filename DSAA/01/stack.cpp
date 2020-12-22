#include<bits/stdc++.h>
using namespace std;

template<class T>
class SeqStack
{
    unsigned int n;
    unsigned int Size;
    T * Bottom;
public:
    SeqStack()
    {
        n=0;
        Size=1;//数组的大小，初始为1，动态增加
        Bottom =new T[Size];
    }
    void push(const T &value)
    {
        if(n>=size)
        {
            int * tmp=Bottom;
            Bottom=new T[size*=2];
            for(int i=0;i<n;i++) Bottom[i]=tmp[i];
            delete[] tmp;
        }
        Bottom[n++]=value;
    }
    int size()
    {
        return n;
    }
    void pop()
    {
        if(n==0) return;
        n--;
    }
    void pop(T & value)
    {
        value=-1;
        if(n==0) return;
        value=Bottom[--n];
    }
    int top()
    {
        if(n==0) return -1;
        return Bottom[n-1];
    }
};

template<class T>
class LinkStack
{
    int n;
    struct node{
        //内置的结构体
        node* next;
        T value;
    };
    node* head;
public:
    LinkStack():n(0),head(NULL){};
    void push(const T& new_value)
    {
        node* new_node=new node;
        new_node->next=head;
        new_node->value=new_value;
        head=new_node;
        n++;
    }
    T top()
    {
        if(head!=NULL)
            return head->value;
        return -1;
    }
    int size()
    {
        return n;
    }
    void pop()
    {
        if(head==NULL) return;
        node*tmp=head;
        head=head->next;
        delete tmp;
    }
    void pop(T& value)
    {
        if(head==NULL) return;
        node*tmp=head;
        head=head->next;
        value=tmp->value;
        delete tmp;
    }
    ~LinkStack()
    {
        while(head!=NULL){
            node*tmp=head;
            head=head->next;
            delete tmp;
        }
    }
};

int main()
{
    LinkStack<int> sta;
    for(int i=1;i<=3;i++)
    {
        cout<<"push "<<i<<endl;
        sta.push(i);
        cout<<"the top is"<<sta.top()<<endl;
    }
    for(int i=1;i<=5;i++){
        int x=-1;
        sta.pop(x);
        cout<<"pop "<<x<<' '<<endl;
        cout<<"the top is"<<sta.top()<<endl;
    }
    return 0;
}