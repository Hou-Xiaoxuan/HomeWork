#include<bits/stdc++.h>
using namespace std;
// Floyed，打印出了每一步
int main()
{
    int dis[7][7]=
    {
        {-1,-1,-1,-1,-1,-1,-1},
        {-1,0,2,-1,-1,-1,3},
        {-1,-1,0,3,2,-1,-1},
        {-1,4,-1,0,-1,4,-1},
        {-1,1,-1,-1,0,1,-1},
        {-1,-1,1,-1,-1,0,3},
        {-1,-1,-1,2,5,-1,0},
    };
    int n=6;
    const int inf=0x3f3f3f3f;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            if(dis[i][j]==-1) dis[i][j]=inf;
    for(int k=1;k<=n;k++)
    {
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                dis[i][j]=min(dis[i][j],dis[i][k]+dis[k][j]);
        cout<<"------Dif("<<k<<")------"<<endl;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
            {
                if(dis[i][j]==inf) cout<<'N';
                else cout<<dis[i][j];
                if(j==n) cout<<endl;
            }
    }
}