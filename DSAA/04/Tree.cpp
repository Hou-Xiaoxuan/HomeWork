/*
第4次上机作业（教学周第11周）：
1. 定义二叉树类，实现广度优先遍历算法、深度优先遍历（先序、中序和后序）的递归算法和非递归算法；
2. 实现由先序、中序序列构造二叉树的算法；
3. 实现由后序、中序序列构造二叉树的算法。
*/

#include<iostream>
#include<vector>
#include<stack>
#include<algorithm>
#include<queue>
using namespace std;

using Comparable=char;//类似模板
class Tree
{
private:
    struct TreeNode
    {
        TreeNode(Comparable val=0):value(val),left_child(NULL),right_child(NULL){};
        Comparable value;
        TreeNode* left_child;
        TreeNode* right_child;
    };

    TreeNode* root;

    /*二叉树的构造的私有版本*/
    void pre_mid_build(TreeNode*&node,vector<Comparable>& preorder,vector<Comparable>&midorder)
    {
        /*重复元素的处理实际上就是二叉搜索树中重复左边还是右边的区别，这里统一成右边*/
        if(preorder.empty() or midorder.empty()){
            node=NULL;
            return;
        }
        node=new TreeNode(preorder[0]);
        auto index=find(midorder.begin(),midorder.end(),preorder[0]);//在中序中寻找pre[0]
        if(index==midorder.end()) return;//这句不可能执行啊，除非输入是错的……
        int len_left=index-midorder.begin();//得到左子树的大小

        vector<Comparable> left_pre(preorder.begin()+1,preorder.begin()+len_left+1);
        vector<Comparable> right_pre(preorder.begin()+len_left+1,preorder.end());
        vector<Comparable> left_mid(midorder.begin(),index);
        vector<Comparable> right_mid(index+1,midorder.end());
        
        pre_mid_build(node->left_child,left_pre,left_mid);
        pre_mid_build(node->right_child,right_pre,right_mid);
    }
    
    void post_mid_build(TreeNode*&node,vector<Comparable>& postorder,vector<Comparable>&midorder)
    {
        if(postorder.empty() or midorder.empty()){
            node=NULL;
            return;
        }
        node =new TreeNode(*(postorder.end()-1));
        auto index=find(midorder.begin(),midorder.end(),node->value);
        int len_left=index-midorder.begin();

        vector<Comparable> left_post(postorder.begin(),postorder.begin()+len_left);
        vector<Comparable> right_post(postorder.begin()+len_left,postorder.end()-1);
        vector<Comparable> left_mid(midorder.begin(),index);
        vector<Comparable> right_mid(index+1,midorder.end());

        post_mid_build(node->left_child,left_post,left_mid);
        post_mid_build(node->right_child,right_post,right_mid);
    }
    /*递归深度遍历的私有版本*/
    void postorder_traversal(TreeNode* node,vector<Comparable>& array)
    {
        if(node==NULL) return;
        if(node->left_child!=NULL) postorder_traversal(node->left_child,array);
        if(node->right_child!=NULL) postorder_traversal(node->right_child,array);
        array.push_back(node->value);
    }
    void inorder_traversal(TreeNode* node,vector<Comparable>& array)
    {
        if(node==NULL) return;
        if(node->left_child!=NULL) inorder_traversal(node->left_child,array);
        array.push_back(node->value);
        if(node->right_child!=NULL) inorder_traversal(node->right_child,array);
    }
    void preorder_traversal(TreeNode* node,vector<Comparable>& array)
    {
        if(node==NULL) return;
        array.push_back(node->value);
        if(node->left_child!=NULL) preorder_traversal(node->left_child,array);
        if(node->right_child!=NULL) preorder_traversal(node->right_child,array);
    }

public:
    Tree():root(NULL){};
    /*树的构造*/
    void pre_mid_build(vector<Comparable>& preorder,vector<Comparable>&midorder)
    {
        if(root!=NULL){//树已经有元素了，则不能调用build函数
            return;
        }
        // 由先序和中序确定二叉树
        pre_mid_build(root,preorder,midorder);
    }
    void post_mid_build(vector<Comparable>& postorder,vector<Comparable>&midorder)
    {
        if(root!=NULL){
            return;
        }
        // 由后序和中序确定二叉树
        post_mid_build(root,postorder,midorder);
    }

    /*二叉树的递归遍历*/
    void preorder_traversal(vector<Comparable>& array){
        preorder_traversal(root,array);
    }
    void inorder_traversal(vector<Comparable>& array){
        inorder_traversal(root,array);
    }
    void postorder_traversal(vector<Comparable>& array){
        postorder_traversal(root,array);
    }

    /*非递归遍历*/
    void preorder_NR(vector<Comparable>&array)
    {
        if(root==NULL) return;
        stack<TreeNode*> sta;
        sta.push(root);
        TreeNode* st=NULL;
        while(!sta.empty())
        {
            if(st==NULL){
                st=sta.top();
                sta.pop();
            }
            while(st!=NULL){
                array.push_back(st->value);
                if(st->right_child!=NULL) sta.push(st->right_child);
                st=st->left_child;
            }
        }
    }
    void inorder_NR(vector<Comparable>&array)
    {
        if(root==NULL) return;
        stack<TreeNode*> sta;
        TreeNode* st=root;
        sta.push(st);
        st=st->left_child;
        while(!sta.empty())
        {
            if(st==NULL){
                st=sta.top();
                sta.pop();
                // 完成mid的访问，并转到右节点
                array.push_back(st->value);
                st=st->right_child;
            }
            while(st!=NULL){
                // 保存节点，去进行左节点遍历
                sta.push(st);
                st=st->left_child;
            }
        }
    }
    void postorder_NR(vector<Comparable>&array)
    {
        /*
        *网上关于非递归后序遍历版本很多
        *最简单的莫过于增加一个指针来标记访问了几次。但是那样需要更待节点保存的信息数量，本人认为不是很好
        *还有一种使用辅助栈，将顺序弹出到辅助栈中，虽没有详细研究，但是在过程中保存了所有节点，空间太大
        *最后参考课本，使用pre指针来判断是否是第二次访问。
        */
        if(root==NULL) return;
        stack<TreeNode*>sta;
        TreeNode* pre=NULL;
        TreeNode* st=root;
        sta.push(st);
        st=st->left_child;
        //当前节点没有右孩子(到底了)/右孩子刚刚被访问过(轮到了)，则访问该节点
        while (!sta.empty())
        {
            if(st==NULL){
                st=sta.top();
                sta.top();
            }
            while(st->left_child!=NULL){
                // 优先访问左节点
                sta.push(st);
                st=st->left_child;
            }
            // 若当前节点符合要求，则访问。
            while(st!=NULL and (st->right_child==NULL or st->right_child==pre))
            {
                array.push_back(st->value);
                pre=st;
                if(sta.empty()) return; //若访问结束，则返回
                st=sta.top();
                sta.pop();
            }
            // 否则，保存节点，进行右节点访问
            sta.push(st);
            st=st->right_child;
        }
    }

    void BFS(vector<Comparable>&array)
    {
        queue<TreeNode*> que;
        que.push(root);
        while(!que.empty())
        {
            auto st=que.front();
            que.pop();
            if(st==NULL) continue;
            array.push_back(st->value);

            que.push(st->left_child);
            que.push(st->right_child);
        }
    }
};

int main()
{
    // vector<Comparable> pre({3,1,2,3,4,5,6});
    // vector<Comparable> mid({1,2,3,3,4,5,6});
    // vector<Comparable> post({2,1,6,5,4,3,3});
    vector<Comparable> pre({'A','B','E','C','D','F','G','H','I','J'});
    vector<Comparable> mid({'E','B','C','D','A','F','H','I','G','J'});

    Tree tree;
    tree.pre_mid_build(pre,mid);
    // tree.post_mid_build(post,mid);
    
    // 验证前序
    cout<<"pre-order: \n";
    vector<Comparable> result;
    tree.preorder_NR(result);
    for(auto i:result) cout<<i<<' ';
    cout<<endl;
    // 验证中序
    cout<<"in-order: \n";
    vector<Comparable>().swap(result);
    tree.inorder_traversal(result);
    for(auto i:result) cout<<i<<' ';
    cout<<endl;
    // 验证后序
    cout<<"past-order: \n";
    vector<Comparable>().swap(result);
    tree.postorder_traversal(result);
    for(auto i:result) cout<<i<<' ';
    cout<<endl;
    // 验证BFS
    cout<<"BFS: \n";
    vector<Comparable>().swap(result);
    tree.BFS(result);
    for(auto i:result) cout<<i<<' ';
    cout<<endl;
    return 0;
}