#include<iostream>
using namespace std;

/*
*题目要求：有用户输入n个10以内的树，没输入i就把他插入到i号队列，最后把10个队列中的非空队列按序号从小到大结成一条链，输出链中的所有元素
*队列由自己实现，采用链式队列，仅实现push，front，pop，empty四个功能
*功能比较简单，不实用模板
*拼接时，通过将所有元素拼接成新的队列实现
*/

class My_queue
{
    const int inf=0x3f3f3f3f;
    struct node
    {
        node(int v):value(v),next(NULL){};
        int value;
        node* next;
    };
    int Size;
    node *head;
    node *current;
public:
    My_queue():Size(0),head(NULL),current(NULL){};
    void push(int value)
    {
        node* new_node=new node(value);
        if(Size==0){
            head=new_node;
            current=head;
            Size++;
            return;
        }
        current->next=new_node;
        current=new_node;
        Size++;
    }
    void pop()
    {
        if(Size==0) return;
        node* st=head;
        head=head->next;
        delete st;
        Size--;
    }
    int front()
    {
        if(Size==0) return inf;
        return head->value;
    }
    bool empty(){
        return Size==0;
    }
    ~My_queue()
    {
        while(!empty()) pop();
    }
};

int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    int n;
    cout<<"Please input the numbers of input n: \n";
    cin>>n;
    My_queue que[10];
    int x;
    cout<<"Please input "<<n<<" numbers in 0~9 : \n";
    for(int i=1;i<=n;i++){
        cin>>x;
        que[x].push(x);
    }
    My_queue ans;
    for(int i=0;i<=9;i++)
    {
        while(!que[i].empty()){
            ans.push(que[i].front());
            que[i].pop();
        }
    }
    cout<<"After combine,the list is: ";
    while(!ans.empty()){
        cout<<ans.front()<<" ";
        ans.pop();
    }
    cout<<endl;

}