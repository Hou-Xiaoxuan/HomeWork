#include<bits/stdc++.h>
using namespace std;
/*
*在s里暴力的寻找p，找到了输出YES和匹配到的首字母位置(从1计数),否则输出NO
测试用例：
input:
abcabcab cabc
abcabcabc caba
output:
Yes, from the index 3!
No!
*/
const int N=1e6;
char p[N];//模式串
char s[N];
bool check(char* s,char *p)
{
    int len_s=strlen(s);
    int len_p=strlen(p);
    if(len_s<len_p) return false;
    for(int i=0;i<len_p;i++)
    {
        if(s[i]==p[i]) continue;
        else return false;
    }
    return true;
}
int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    while(cin>>s>>p)
    {
        int len_p=strlen(p);
        int len_s=strlen(s);
        bool flag=false;
        for(int i=0;i<len_s and len_p<=len_s-i;i++)
        {
            if(check(s+i,p)){
                cout<<"Yes, ";
                cout<<"from the index "<<i+1<<"!"<<endl;
                flag=true;
            }
        }
        if(flag==false) cout<<"No!"<<endl;
    }

}