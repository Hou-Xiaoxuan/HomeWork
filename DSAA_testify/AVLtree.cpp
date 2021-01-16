#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

using Comparable=int;
class AVLTree
{
private:
    struct AVLNode
    {
        AVLNode(const Comparable& val):value(val){};
        AVLNode*left_child;
        AVLNode*right_child;
        size_t height;
        Comparable value;
    };
    
    AVLNode* root;

    /*中序遍历*/
    void inorder_traversal(AVLNode* node,vector<Comparable>& array)
    {
        if(node==NULL) return;
        if(node->left_child!=NULL) inorder_traversal(node->left_child,array);
        array.push_back(node->value);
        if(node->right_child!=NULL) inorder_traversal(node->right_child,array);
    }

    /*旋转操作*/
    void rote_with_left(AVLNode* &k1)
    {
        // 右旋(左支旋转)
        AVLNode* k2=k1->left_child;//这里不能用引用
        k1->left_child=k2->right_child;
        k2->right_child=k1;
        k1->height=max(k1->left_child->height,k1->right_child->height)+1;
        k2->height=max(k2->left_child->height,k2->right_child->height)+1;
        k1=k2;
    }
    void rote_with_right(AVLNode* &k1)
    {
        // 左旋
        AVLNode* k2=k1->right_child;
        k1->right_child=k2->left_child;
        k2->left_child=k1;
        k1->height=max(k1->left_child->height,k1->right_child->height)+1;
        k2->height=max(k2->left_child->height,k2->right_child->height)+1;
        k1=k2;
    }
    void double_with_left(AVLNode* &k1)
    {
        // 左右双旋
        rote_with_right(k1->left_child);
        rote_with_left(k1);
    }
    void double_with_right(AVLNode* &k1)
    {
        // 右左双旋
        rote_with_left(k1->right_child);
        rote_with_right(k1);
    }

    /*插入*/
    void insert(AVLNode*&node,const Comparable& val)
    {
        

    }

    /*删除*/
    void earse(AVLNode*&node,const Comparable& val)
    {


    }
public:
    AVLTree():root(NULL){};
    void insert(const Comparable& val){
        insert(root,val);
    }
    void inorder_traversal(vector<Comparable>& array){
        inorder_traversal(root,array);
    }
};