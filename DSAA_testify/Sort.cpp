/*实现八种排序算法*/
#include<iostream>
#include<algorithm>
using std::cin;
using std::cout;
using std::endl;
namespace Direct_Insert
{
    /*直接插入排序*/
    // using T=int;

    template<typename T>
    bool Compare(const T&a,const T&b){
        return a<b;
    }
    /*
    *区间排序，左闭右开
    *只定义了排序参数为引用的情况，还可以重载指针，赋值两种情况
    */
   template<typename T>
    void sort(T*Begain,T*End,bool(*cmp)(const T&a,const T&b)=Compare)
    {
        int n=End-Begain;
        for(int i=1;i<n;i++)//插入i
        {
            for(int j=0;j<i;j++)
            {
                if(cmp(Begain[i],Begain[j])==true)//i插入第一个比自己大的数前面
                {
                    T tmp=Begain[i];
                    for(int k=i;k>j;k--)//移动
                        Begain[k]=Begain[k-1];
                    Begain[j]=tmp;
                    break;
                }
            }
        }
    }
}

namespace Binary_Indert
{
    /*折半插入排序*/
    // using T=int;

    template<typename T>
    bool Compare(const T&a,const T&b){
        return a<b;
    }
    template<typename T>
    T* upper_bound(T*Begain,T*End,const T&value,bool(*cmp)(const T&a,const T&b)=Compare)
    {
        /*二分查找<value的上界,找不到返回End*/
        int n=End-Begain;
        int l=0,r=n-1;
        T* ret=End;//初始指向最后
        while(l<=r)
        {
            int mid=(l+r)>>1;
            if(Begain[mid]<value)
            {
                l=mid+1;
            }
            else
            {
                r=mid-1;
                ret=Begain+mid;
            }
        }
        return ret;
    }
    /*
    *区间排序，左闭右开
    *只定义了排序参数为引用的情况，还可以重载指针，赋值两种情况
    */
    template<typename T>
    void sort(T*Begain,T*End,bool(*cmp)(const T&a,const T&b)=Compare)
    {
        int n=End-Begain;
        for(int i=1;i<n;i++)
        {
            int index=upper_bound<T> (Begain,Begain+i,Begain[i],cmp)-Begain;
            T tmp=Begain[i];
            for(int k=i;k>index;k--)
                Begain[k]=Begain[k-1];
            Begain[index]=tmp;
        }
    }
}

namespace Shell_Insert
{
    /*希尔插入排序*/
    // using T=int;

    template<typename T>
    bool Compare(const T&a,const T&b){
        return a<b;
    }
    /*为了得到差不多的精度，采用每次除3向下取整+1*/
    template<typename T>
    void sort(T*Begain,T*End,bool(*cmp)(const T&a,const T&b)=Compare)
    {
        int n=End-Begain;
        int segment=n;
        while(segment>1)
        {
            segment=segment/3+1;
            // 执行普通的插入排序
            for(int i=segment;i<n;i+=segment)
            {
                for(int j=0;j<i;j+=segment)
                {
                    if(cmp(Begain[i],Begain[j])==true)
                    {
                        T tmp=Begain[i];
                        for(int k=i;k>j;k-=segment)
                            Begain[k]=Begain[k-segment];
                        Begain[j]=tmp;
                        break;
                    }
                }
            }
        }
    }
}

namespace Bubble_Swap
{
    /*冒泡排序*/
    template<typename T>
    // using T=int;
    bool Compare(const T&a,const T&b){
        return a<b;
    }

    template<typename T>
    void sort(T*Begain,T*End,bool(*cmp)(const T&a,const T&b)=Compare)
    {
        int n=End-Begain;
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n-i-1;j++)
            {
                if(cmp(Begain[j],Begain[j+1])==false){
                    T tmp=Begain[j+1];
                    Begain[j+1]=Begain[j];
                    Begain[j]=tmp;
                }
            }
        }
    }
}

namespace Qsort_Swap
{
    /*快速排序*/
    // using T=int;

    template<typename T>
    bool Compare(const T&a,const T&b){
        return a<b;
    }

    template<typename T>
    int partition(T*Begain,T*End,bool(*cmp)(const T&a,const T&b)=Compare)
    {
        /*找到左侧的划分数目并返回*/
        int n=End-Begain;
        int left_index=0,right_index=n-1;
        T stand=Begain[left_index];//默认基准
        while(left_index<right_index)
        {
            //left处的数据已经保存了
            while(left_index<right_index and cmp(stand,Begain[right_index])) right_index--;
            // 得到第一个小于stand的右侧元素
            while(left_index<right_index and cmp(stand,Begain[right_index])==cmp(Begain[right_index],stand))
                right_index--;//由于cmp只有小于功能(使用者也可能定义成<=),因此相等元素的判断会出死循环，需要特判相等
            if(left_index<right_index)
                Begain[left_index]=Begain[right_index];
            // right处数据已经保存
            while(left_index<right_index and cmp(Begain[left_index],stand)) left_index++;
            while(left_index<right_index and cmp(stand,Begain[left_index])==cmp(Begain[left_index],stand))
                left_index++;//特判相等
            if(left_index<right_index)
                Begain[right_index]=Begain[left_index];
        }
        Begain[left_index]=stand;//放回保存的元素
        return left_index+1;//此使left=right,数目=index+1
    }

    template<typename T>
    void sort(T*Begain,T*End,bool(*cmp)(const T&a,const T&b)=Compare)
    {
        int n=End-Begain;
        if(n<=1) return;
        int index=partition<T>(Begain,End,cmp);
        sort(Begain,Begain+index,cmp);
        sort(Begain+index,End,cmp);
    }

}

namespace Simple_Select
{
    /*选择排序*/
    template<typename T>
    bool Compare(const T&a,const T&b){
        return a<b;
    }

    template<typename T>
    void sort(T*Begain,T*End,bool(*cmp)(const T&a,const T&b)=Compare)
    {
        int n=End-Begain;

        for(int i=0;i<n;i++)
        {
            int index=i;//最小元素的下标
            for(int j=i+1;j<n;j++)
                if(cmp(Begain[j],Begain[index]))
                    index=j;
            T tmp=Begain[index];
            Begain[index]=Begain[i];
            Begain[i]=tmp;
        }
    }
}

namespace Merge
{
    /*归并排序*/
    // using T=int;

    template<typename T>
    bool Compare(const T&a,const T&b){
        return a<b;
    }

    template<typename T>
    void sort(T*Begain,T*End,bool(*cmp)(const T&a,const T&b)=Compare)
    {
        int n=End-Begain;
        if(n<=1) return;
        int len=n/2;//左边n/2个
        T *Left=new T[len];
        T *Right=new T[n-len];
        for(int i=0;i<len;i++)
            Left[i]=Begain[i];
        for(int i=len;i<n;i++)
            Right[i-len]=Begain[i];
        Merge::sort(Left,Left+len,cmp);
        Merge::sort(Right,Right+n-len,cmp);
        int left_index=0,right_index=0;
        for(int i=0;i<n;i++)
        {
            if(left_index<len and right_index<n-len)
            {
                if(cmp(Left[left_index],Right[right_index]))
                    Begain[i]=Left[left_index++];
                else 
                    Begain[i]=Right[right_index++];   
            }
            else if(left_index<len)
                Begain[i]=Left[left_index++];
            else
                Begain[i]=Right[right_index++];
        }
    }
}

bool cmp(const int &a,const int&b){
    return a>b;
}
int main()
{
    int a[9]={60, 40, 120, 185, 20, 135, 150, 130, 45};
    Bubble_Swap::sort(a,a+9);
    return 0;
}
