#include<bits/stdc++.h>
using namespace std;
#define P pair<int,int>
#define LL long long
#define m_p make_pair
const int inf=2147483647;

int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    int n,m,s;
    while(cin>>n>>m>>s)
    {
        vector<vector<pair<int,int> > > edg(n+1);
        for(int i=1;i<=m;i++){
            int u,v,w;
            cin>>u>>v>>w;
            edg[u].push_back(make_pair(v,w));
        }

        /*Dijskra*/
        vector<int> dis(n+1,inf);
        vector<bool> vis(n+1);
        dis[s]=0;
        priority_queue<P,vector<P>,greater<P>> que;//这一行代码
        que.push(m_p(0,s));
        while(!que.empty())
        {
            P st = que.top();
            que.pop();
            if(vis[st.second]==true) continue;
            vis[st.second]=true;
            
            for(auto v:edg[st.second])
            {
                if(dis[v.first]>st.first+v.second){
                    dis[v.first]=st.first+v.second;
                    que.push(m_p(dis[v.first],v.first));
                }
            }
        }
        for(int i=1;i<=n;i++)
            cout<<dis[i]<<" ";
        cout<<endl;
    }

    return 0;
}

