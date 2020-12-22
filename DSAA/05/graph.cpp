/*
    第5次上机作业（教学周第13周）：
    实现图的DFS算法（递归和非递归）、BFS算法。
*/
/*
测试用例输入：
5 4
1 2 0
1 3 0
1 4 0
3 5 0
*/

#include<iostream>
#include<stack>
#include<queue>
#include<vector>
#include<algorithm>
#include<string.h>
using namespace std;

const int N=1e5;
struct Edg
{
    int value;
    int to;
    int next;
    Edg(int too=0,int val=0):to(too),value(val),next(0){};
}edg[N+5];
int  Head[N+5]={0},tot=0;
void add(int u,int v,int value)
{
    edg[++tot]=Edg(v,value);
    edg[tot].next=Head[u];
    Head[u]=tot;
}

//非递归dfs并打印dfs序
bool vis[N+5];
void dfs_stack(int n)
{
    memset(vis,0,sizeof(bool)*(n+5));
    stack<int> sta;
    sta.push(1);
    while(!sta.empty())
    {
        int st=sta.top();
        sta.pop();
        if(vis[st]==true) continue;
        vis[st]=true;
        // cout<<st<<" ";
        for(int i=Head[st];i>0;i=edg[i].next)
        {
            if(vis[edg[i].to]==true) continue;
            else
            {
                sta.push(edg[i].to);
            }
        }
    }
    cout<<"The result of DFS(with stack):"<<endl;
    if(count(vis,vis+n+1,true)==n) cout<<"All nodes were visited!"<<endl;
    else cout<<"No all the nodes were visited in BFS, the numbers of node that were visited is: "<<count(vis,vis+n+1,true)<<endl;
    cout<<endl;
}

bool dfs(int n,int st)
{
    if(count(vis,vis+n+1,true)==0){
        memset(vis,0,sizeof(bool)*(n+5));
        vis[st]=true;
    }
    if(count(vis,vis+n+1,true)==n){
        return true;
    }
    for(int i=Head[st];i>0;i=edg[i].next)
    {
        if(vis[edg[i].to]==true) continue;
        vis[edg[i].to]=true;
        if(dfs(n,edg[i].to)==true) return true;
    }
    return false;
}
void bfs(int n)
{
    if(n==0) return;
    memset(vis,0,sizeof(bool)*(n+5));
    queue<int> que;
    que.push(1);
    vis[1]=true;
    while(!que.empty())
    {
        int u=que.front();
        que.pop();
        vis[u]=true;
        for(int i=Head[u];i>0;i=edg[i].next)
        {
            if(vis[edg[i].to]==true) continue;
            else que.push(edg[i].to);
        }
    }
    cout<<"The result of BFS:"<<endl;
    if(count(vis,vis+n+1,true)==n) cout<<"All nodes were visited!"<<endl;
    else cout<<"No all the nodes were visited in BFS, the numbers of node that were visited is: "<<count(vis,vis+n+1,true)<<endl;
    cout<<endl;
}
int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG
    // 得到点数和边数
    int n,m;
    cin>>n>>m;
    int u,v,val;
    // 默认双向边
    for(int i=1;i<=m;i++){
        cin>>u>>v>>val;
        add(u,v,val);
        add(v,u,val);
    }
    cout<<"The result of DFS:"<<endl;
    if(dfs(n,1)) cout<<"All nodes were visited in this DFS"<<endl;
    else cout<<"No Were nodes were visited"<<endl;
    cout<<endl;
    dfs_stack(n);
    bfs(n);


    return 0;
}