/*树的实现(默认二叉树查找)*/
#include<iostream>
#include<vector>
#include<stack>
#include<iso646.h>
using namespace std;

// 模板不方便调试，使用Comparable代替
using Comparable=char;
class BinarySearchTree
{
    struct TreeNode
    {
        TreeNode(Comparable val):value(val),left_child(NULL),right_child(NULL){};
        Comparable value;
        TreeNode* left_child;
        TreeNode* right_child;
    };
    TreeNode* root;

    /*递归深度遍历的实现*/
    void preorder_traversal(TreeNode* node,vector<Comparable>& array)
    {
        if(node==NULL) return;
        array.push_back(node->value);
        if(node->left_child!=NULL) preorder_traversal(node->left_child,array);
        if(node->right_child!=NULL) preorder_traversal(node->right_child,array);

    }
    void inorder_traversal(TreeNode* node,vector<Comparable>& array)
    {
        if(node==NULL) return;
        if(node->left_child!=NULL) inorder_traversal(node->left_child,array);
        array.push_back(node->value);
        if(node->right_child!=NULL) inorder_traversal(node->right_child,array);
    }
    void postorder_traversal(TreeNode* node,vector<Comparable>& array)
    {
        if(node==NULL) return;
        if(node->left_child!=NULL) postorder_traversal(node->left_child,array);
        if(node->right_child!=NULL) postorder_traversal(node->right_child,array);
        array.push_back(node->value);
    }

    /*插入的实现*/
    void insert(Comparable val,TreeNode* &node)
    {
        if(node==NULL) node=new TreeNode(val);
        else if(val<node->value) insert(val,node->left_child);
        else insert(val,node->right_child);
    }
    /*查找的实现*/
    const Comparable& find_max(TreeNode* node)
    {
        while(node->right_child!=NULL) node=node->right_child;
        return node->value;
    }
    const Comparable& find_min(TreeNode* node)
    {
        while(node->left_child!=NULL) node=node->left_child;
        return node->value;
    }

public:
    /*构造*/
    BinarySearchTree():root(NULL){};
    BinarySearchTree(vector<Comparable>&array):root(NULL){
        build(array);
    }
    void build(vector<Comparable>&array)
    {
        /*利用a数组的数据建立一棵二分查找树*/
        for(auto &i:array) insert(i);
    }

    /*插入*/
    void insert(Comparable val){
        insert(val,root);
    }

    /*查找*/
    const Comparable& find_max(){
        return find_max(root);
    }
    const Comparable& find_min(){
        return find_min(root);
    }
    size_t count(const Comparable& var)
    {
        // 按照我的处理，重复元素会插入到右侧
        size_t ret_number=0;
        if(root==NULL) return ret_number;
        TreeNode* node=root;
        while(node!=NULL)
        {
            if(var==node->value) ret_number++;
            
            if(var<node->value) node=node->left_child;
            else node=node->right_child;
        }
        return ret_number;
    }
    // const Comparable& find(Comparable val,bool &flag)
    // {
    //     /*
    //     *在写这个函数的时候我真的意识到了，迭代器是个多么香的东西
    //     *没有迭代器的我，只能增加一个累赘的flag来表示是否找到了
    //     *舍弃了，这个find没有任何必要，不如直接写count
    //     */
    //     flag=true;
    //     // 返回int型没有任何用处，但是改成模板后返回结构体等有用
    //     TreeNode *node=root;
    //     while(node!=NULL)
    //     {
    //        if(node->value==val) return node->value;
    //        if(val<node->value) node=node->left_child;
    //        else node=node->right_child;
    //     }
    //     flag=false;
    //     return val;
    // }
    /*删除*/
    bool remove(const Comparable& val){
        return remove(val,root);
    }
    bool remove(const Comparable& val, TreeNode* &node)
    {
        // 没找到
        if(node==NULL) return false;
        // 在子节点中查找
        if(val<node->value) return remove(val,node->left_child);
        else if(val>node->value) return remove(val,node->right_child);
        // 找到了待删除节点，三种情况
        if(node->left_child==NULL and node->right_child==NULL){
            delete node;
            node=NULL;
        }
        else if(node->left_child==NULL or node->right_child==NULL){
            TreeNode* tmp_remove=node;
            node = node->left_child==NULL?node->right_child:node->right_child;
            delete tmp_remove;
        }
        else
        {
            int replace=find_max(node->right_child);
            remove(replace,node->right_child);
            node->value=replace;
        }
        return true;
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
};

int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    int n; cin>>n;
    vector<Comparable> array(n);
    for(int i=0;i<n;i++) cin>>array[i];
    BinarySearchTree& tree= *(new BinarySearchTree(array));

    cout<<"先序遍历："<<endl;
    vector<Comparable>().swap(array);
    tree.preorder_traversal(array);
    for(auto i:array) cout<<i<<' ';
    cout<<endl;
    cout<<"非递归先序遍历："<<endl;
    vector<Comparable>().swap(array);
    tree.preorder_NR(array);
    for(auto i:array) cout<<i<<' ';
    cout<<endl;

    cout<<"中序遍历："<<endl;
    vector<Comparable>().swap(array);
    tree.inorder_traversal(array);
    for(auto i:array) cout<<i<<' ';
    cout<<endl;
    cout<<"非递归中序遍历："<<endl;
    vector<Comparable>().swap(array);
    tree.inorder_NR(array);
    for(auto i:array) cout<<i<<' ';
    cout<<endl;
    
    cout<<"后序遍历："<<endl;
    vector<Comparable>().swap(array);
    tree.postorder_traversal(array);
    for(auto i:array) cout<<i<<' ';
    cout<<endl;
    cout<<"非递归后序遍历："<<endl;
    vector<Comparable>().swap(array);
    tree.postorder_NR(array);
    for(auto i:array) cout<<i<<' ';
    cout<<endl;
    
    cout<<"元素 a 的数量是:"<<endl;
    cout<<tree.count('a')<<endl;
    cout<<"元素 b 的数量是:"<<endl;
    cout<<tree.count('b')<<endl;

    cout<<"最大值是："<<endl;
    cout<<tree.find_max()<<endl;
    cout<<"最小值是："<<endl;
    cout<<tree.find_min()<<endl;

    cout<<"分别删除最大值和最小值之后"<<endl;
    tree.remove(tree.find_max());
    tree.remove(tree.find_min());
    cout<<"最大值是："<<endl;
    cout<<tree.find_max()<<endl;
    cout<<"最小值是："<<endl;
    cout<<tree.find_min()<<endl;
    return 0;
}
