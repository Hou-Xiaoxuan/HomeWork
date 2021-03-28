#include<bits/stdc++.h>
using namespace std;
using Comparable=int;
// 10 5 17 3 6
// 逻辑是对了……但是手写的好乱啊……不知道能不能给分……
// 算了，估计是没有给分
class BinarySearchTree
{
    struct TreeNode
    {
        TreeNode(Comparable val):value(val),left_child(NULL),right_child(NULL){};
        Comparable value;
        TreeNode* left_child;
        TreeNode* right_child;
    };
public:
    TreeNode* root;
    BinarySearchTree():root(NULL){};
    void insert(Comparable val,TreeNode* &node)
    {
        if(node==NULL) node=new TreeNode(val);
        else if(val<node->value) insert(val,node->left_child);
        else insert(val,node->right_child);
    }
    void visit()
    {
        auto st=root;
        stack<TreeNode*> sta;
        while(st->left_child!=NULL){
            sta.push(st);
            st=st->left_child;
        }
        if(st->left_child==nullptr and st->right_child==nullptr) cout<<st->value<<endl;
        while (sta.empty()==false)
        {
            st=sta.top()->right_child;
            sta.pop();
            if(st==nullptr) continue;
            if(st->left_child==nullptr and st->right_child==nullptr) cout<<st->value<<endl;
            else
            {
                while(st->left_child!=nullptr){
                    sta.push(st);
                    st=st->left_child;
                }
                if(st->left_child==nullptr and st->right_child==nullptr) cout<<st->value<<endl;
            }
        }
    }
    /*
        10        
       /  \       
      5   17      
     / \          
    3   6         
    */
};

int main()
{
    BinarySearchTree tree;
    vector<int> array={10,5,17,3,6};
    for(auto i:array) tree.insert(i,tree.root);
    tree.visit();
    return 0;
}
