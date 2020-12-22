#include<bits/stdc++.h>
using namespace std;

const int N=1e6;
char s[N+5],t[N+5];
char Strcmp(char* s,char* t)
{
    int len_s=strlen(s),len_t=strlen(t);
    for(int i=0;i<len_s and i<len_t;i++)
    {
        if(s[i]<t[i]) return '<';
        else if(s[i]==t[i]) continue;
        else return '>';
    }
    if(len_t<len_s) return '>';
    else if(len_t==len_s) return '=';
    else return '<';
}
int main()
{
    // map<int,char> ma{{-1,'<'},{0,'='},{1,'>'}};
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    while(cin>>s>>t)
    {
        cout<<s<<" "<<Strcmp(s,t)<<" "<<t<<endl;
        cout<<"STL: "<<(string(s)<string(t)?'<':(string(t)==string(s)?'=':'>'))<<endl;
    }

}