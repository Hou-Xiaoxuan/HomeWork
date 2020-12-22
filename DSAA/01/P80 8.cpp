#include<bits/stdc++.h>
using namespace std;

/*
*题目要求：从循环双链表中删除第一个元素为x的节点
*解答思路：
*1、第一个默认为next方向遇到的第一个，找不到是返回false
*2、每次插入时，插入到当前节点的写一个位置
*3、删除后，当前节点会随机更新
*4、需要自定义类型重载==(必选)和<<(可选，用于输出)
*/
template<class T>
class My_List
{
    struct node
    {
        T value;
        node*front;
        node* next;
        node(const T &val=0):value(val),front(NULL),next(NULL){};
    };
    node* Pr;
    int n;
public:
    My_List():Pr(NULL),n(0){};
    void insert(const T& new_value)
    {
        if(n==0)
        {
            Pr=new node(new_value);
            Pr->next=Pr;
            Pr->front=Pr;
            n++;
        }
        else
        {
            node *new_node=new node(new_value);
            new_node->next=Pr->next;
            Pr->next->front=new_node;
            Pr->next=new_node;
            new_node->front=Pr;
            n++;
        }
        
    }
    // 打印函数需要重载<<
    void print()
    {
        if(n==0){
            cout<<"empty!"<<endl;
            return;
        }
        else
        {
            node*tmp=Pr;
            cout<<Pr->value<<" ";
            tmp=Pr->next;
            while(tmp!=Pr){
                cout<<tmp->value<<" ";
                tmp=tmp->next;
            }
            cout<<endl;
        }
    }
    
    bool del(int x)
    {
        if(n==0) return false;
        node *tmp=NULL;
        if(Pr->value==x) tmp=Pr;//如果当前恰好为x
        else//寻找x
        {
            tmp=Pr->next;
            while(tmp!=Pr){
                if(tmp->value==x) break;
                tmp=tmp->next;
            }
        }
        if(tmp==NULL) return false;//没找到直接返回
        // 分情况执行删除
        if(n==1){
            delete tmp;
            Pr=NULL;
        }
        else
        {
            node* l=tmp->front,*r=tmp->next;
            l->next=r;
            r->front=l;
            Pr=tmp->next;
            delete tmp;
        }
        n--;
        return true;
    }
    ~My_List()
    {
        while(Pr!=NULL) del(Pr->value);
    }
};

int main()
{
    My_List<int> lis;
    lis.insert(1);
    lis.insert(2);
    lis.insert(3);
    lis.insert(4);
    cout<<"The liset now:";
    lis.print();
    for(int i=1;i<=5;i++){
        cout<<"After delete the first value "<<i<<":";
        lis.del(i);
        lis.print();
    }
    return 0;
}