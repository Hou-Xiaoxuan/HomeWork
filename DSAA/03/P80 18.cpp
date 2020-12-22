#include<bits/stdc++.h>
using namespace std;
/*
*使用kmp去匹配，返回最后一次匹配信息；
*匹配不到返回-1
*/
int find_last_substr(string &str,string &substr)
{
    str.insert(str.begin(),'0');
    substr.insert(substr.begin(),'0');
    vector<int> kmp(substr.size()+1);
    // get next array
    int j=0;
    for(int i=2;i<(int)substr.size();i++)
    {
        while(j>0 and substr[j+1]!=substr[i]) j=kmp[j];
        if(substr[j+1]==substr[i]) j++;
        kmp[i]=j;
    }
    // match
    j=0;
    kmp[1]=0;
    int ans=-1;
    for(int i=1;i<(int)str.size();i++)
    {
        while(str[i]!=substr[j+1] and j>0) j=kmp[j];
        if(str[i]==substr[j+1]) j++;
        if(j==(int)substr.size()-1){
            // 匹配成功
            ans=i-j+1;
        }
    }
    return ans;
}
int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    string a,b;
    while(cin>>a>>b)
        cout<<"the substr's last occur is: "<<find_last_substr(a,b)<<endl;
    return 0;
}