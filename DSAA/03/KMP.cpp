#include<bits/stdc++.h>
using namespace std;

int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    string s,p;
    while(cin>>s>>p)
    {
        s.insert(s.begin(),'0');
        p.insert(p.begin(),'0');
        vector<int> kmp(p.size()+1);

        // get next array
        int j=0;
        for(int i=2;i<(int)p.size();i++)
        {
            while(j>0 and p[j+1]!=p[i]) j=kmp[j];
            if(p[j+1]==p[i]) j++;
            kmp[i]=j;
        }

        // match
        j=0;
        kmp[1]=0;
        for(int i=1;i<(int)s.size();i++)
        {
            while(s[i]!=p[j+1] and j>0) j=kmp[j];
            if(s[i]==p[j+1]) j++;
            if(j==(int)p.size()-1){
                // 匹配成功
                cout<<"Yes,";
                cout<<"from the index "<<i-j+1<<"!"<<endl;
                break;
            }
        }
        if(j!=p.size()-1) cout<<"No!"<<endl;
    }
    return 0;
}