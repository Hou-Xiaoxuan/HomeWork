#include<bits/stdc++.h>
using namespace std;
// 偷懒……直接使用vector实现了
// 默认大根堆
template<class Type>
struct Compair
{
    bool operator()(const Type& a,const Type& b ){
        return a<b;
    }
};

template<class Type,class Cmp=Compair<Type> >
class Heap
{
    size_t Capacity;
    vector<Type> array;//从1开始计数
    Cmp cmp;
    // 上浮,需要保证除pos外结构为堆
    void sift_up(size_t pos)
    {
        while(pos>1)
        {
            size_t parent=pos/2;
            if(cmp(array[parent],array[pos])==true)
                swap(array[pos],array[parent]);
            else 
                break;
            pos=parent;
        }
    }
    // 下沉，需要保证pos的子树为堆
    void sift_down(size_t pos)
    {
        while(pos<=Capacity)
        {
            size_t child=pos<<1;
            if(child>Capacity) break;
            if(child+1<=Capacity and cmp(array[child],array[child+1])==true)
                child++;
            // child 要去left和right中较大的那个
            if(cmp(array[pos],array[child])){
                swap(array[pos],array[child]);
                pos=child;
            }
            else break;
        }
    }
public:
    Heap(){
        array.push_back(Type());
        Capacity=0;
    }
    Heap(vector<Type>&data)
    {
        array.assign(data.size()+1,0);
        Capacity=data.size();
        for(size_t i=0;i<Capacity;i++)
            array[i+1]=data[i];
        // 初始化，从后往前sift_down,保证子树为堆的情况下sift_down;
        for(size_t i=Capacity;i>=1;i--)
            sift_down(i);
    }
    void push(const Type& value)
    {
        array.push_back(value);
        Capacity++;
        sift_up(Capacity);
    }
    void pop()
    {
        if(Capacity<1) return;
        swap(array[1],array[Capacity]);
        array.pop_back();
        Capacity--;
        sift_down(1);
    }
    Type top()
    {
        if(Capacity<1){
            throw 0;
        }
        return array[1];
    }
    bool empty()
    {
        return Capacity<1;
    }
    ~Heap(){
        vector<Type>().swap(array);
    }
};

int main()
{
    struct ex
    {
        int a,b,c;
        ex(){
            a=rand()%100;
            b=rand()%100;
            c=rand()%100;
        }
    };
    struct cmp
    {
        bool operator()(ex x,ex y)
        {
            return x.c>y.c;
        }
    };
    Heap<ex,cmp> que;
    for(size_t t=1;t<=5;t++)
        que.push(ex());
    while(!que.empty())
    {
        auto st=que.top();
        que.pop();
        cout<<st.a<<' '<<st.b<<' '<<st.c<<endl;
    }
    
    return 0;
}
