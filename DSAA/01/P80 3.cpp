#include<bits/stdc++.h>
using namespace std;

/*
*题目要求：给定一个不带头节点的单链表，写出将链表倒置的算法
*解答思路：
*1、题目并没有给定链表的给定形式，因此自定义为每次更新头节点的形式
*2、链表保存元素个数
*3、链表插入时，给定的是元素的值而不是指针
*4、链表只实现增加节点和倒置两种功能
*
*/
template<class T>
class My_List
{
    int n;
    struct node{
        //内置的结构体
        node* next;
        T value;
    };
    node* head;
public:
    My_List():n(0),head(NULL){};
    void insert(const T& new_value)
    {
        node* new_node=new node;
        new_node->next=head;
        new_node->value=new_value;
        head=new_node;
        n++;
    }
    void invert()
    {
        if(n<=1) return;
        else if(n==2){
            node* tmp=head->next;
            tmp->next=head;
            head->next=NULL;
            head=tmp;
            return;
        }
        // 最容易乱的一部分需要理清关系
        node *left,*middle,*right;
        left=head;
        middle=left->next;
        right = left->next->next;
        left->next=NULL;
        while(right!=NULL)
        {
            middle->next=left;
            left=middle;
            middle=right;
            right=right->next;
        }
        middle->next=left;
        head=middle;
    }
    void print()//T需要重载<<才能直接输出
    {
        cout<<"print the list:";
        node* pointer=head;
        while(pointer!=NULL){
            cout<<pointer->value;
            if(pointer->next!=NULL) cout<<"->";
            pointer=pointer->next;
        }
        cout<<endl;
    }
    ~My_List()
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

    My_List<int> lis;
    lis.insert(1);
    lis.insert(2);
    lis.insert(3);
    lis.insert(4);
    lis.insert(5);
    lis.print();
    cout<<"invert!"<<endl;
    lis.invert();
    lis.print();
    return 0;
}
