/*2.5 课后习题

a.	设一棵二叉搜索树以二叉链表表示，试编写有关二叉树的递归算法
	int degree1(binarytreenode* p)							//2.5-a-i 统计度为1的结点
	int degree2(binarytreenode* p)							//2.5-a-ii 统计二叉树中度为2的结点个数
	int degree3(binarytreenode* p)							//2.5-a-iii 统计二叉树中度为0的结点个数
	int get_height(binarytreenode* p)						//2.5-a-iv 统计二叉树的高度
	int get_max_width()                                 	//2.5-a-v 统计并返回二叉树的宽度（本题不用使用递归）
	int get_max(binarytreenode* p)							//2.5-a-vi 计算二叉树中各结点中的最大元素的值
	void change_children(binarytreenode* p)					//2.5-a-vii 交换每个结点的左子树和右子树？
	void del_leaf(binarytreenode* p)						//2.5-a-viii 从二叉树中删去所有叶结点
	bool wanquan(binarytreenode* pRoot)						//2.5-b 编写算法判定给定二叉树是否为完全二叉树
*/
#include <iostream>
#include<algorithm>
#include <queue>
using namespace std;

class binarytreenode //二叉树结点
{
    int data;

public:
    binarytreenode* leftchild;
    binarytreenode* rightchild;
    binarytreenode(int& d)
    {
        data = d;
        leftchild = NULL;
        rightchild = NULL;
    }
    int& get_data() { return data; }
    void change_data(int& n) { data = n; }
    ~binarytreenode(){};
};

//二叉树
class binarytree {
    binarytreenode* root;
    static int times;
    int size;
public:
    binarytree()
    {
        size = 0;
        root = NULL;
    }
    int get_size() { return size; }
    binarytreenode* get_root() { return root; }
    void creat() //创建二叉树
    {
        // 小于往左插，否则往右插
        binarytreenode* prev;
        cout << "输入int型数据:(以0结束)";
        int temp;
        cin >> temp;
        while (temp != 0) {
            if (root == NULL) {
                root = new binarytreenode(temp);
                size++;
                prev = root;
            } else {
                prev = root;
                size++;
                for (;;) {
                    if (temp < prev->get_data()) {
                        if (prev->leftchild == NULL) {
                            prev->leftchild = new binarytreenode(temp);
                            break;
                        }
                        prev = prev->leftchild;
                    } else {
                        if (prev->rightchild == NULL) {
                            prev->rightchild = new binarytreenode(temp);
                            break;
                        }
                        prev = prev->rightchild;
                    }
                }
            }
            cin >> temp;
        }
    }

    void levelorder() //广度优先遍历
    {
        binarytreenode* p = get_root();
        queue<binarytreenode*> que;
        cout << "广度遍历结果: ";
        if (p != NULL)
            que.push(p);
        else
            cout << "二叉树为空!";
        while (!que.empty()) {
            if (p != NULL) {
                cout << p->get_data() << " ";
                if (p->leftchild != NULL)
                    que.push(p->leftchild);
                if (p->rightchild != NULL)
                    que.push(p->rightchild);
                que.pop();
                if (!que.empty())
                    p = que.front();
            }
        }
        cout << endl;
    }
    /*
	TODO:统计并返回度为1的结点个数
	 */
    int degree1(binarytreenode* p)
    {
        if(p==NULL) return 0;
        if(p->leftchild==NULL and p->rightchild==NULL) return 0;
        int ans=degree1(p->leftchild)+degree1(p->rightchild);
        ans+=(int(p->rightchild==NULL)+int(p->leftchild==NULL))%2;
        return ans;
    }
    /*
	TODO:统计并返回度为2的结点个数
	 */
    int degree2(binarytreenode* p)
    {
        if(p==NULL)
            return 0;
        int ans=degree2(p->leftchild)+degree2(p->rightchild);
        if(p->leftchild!=NULL and p->rightchild!=NULL) ans++;
        return ans;
    }
    /*
	TODO:统计并返回度为0的结点个数
	 */
    int degree0(binarytreenode* p)
    {
        if(p==NULL) return 0;
        else if(p->leftchild==NULL and p->rightchild==NULL) return 1;
        else return degree0(p->leftchild)+degree0(p->rightchild);
    }
    /*
	TODO:统计并返回高度
	 */
    int get_height(binarytreenode* p)
    {
        if(p==NULL) return 0;
        else return 1+max(get_height(p->leftchild),get_height(p->rightchild));
    }
    void get_width(binarytreenode* p, int i, int wide[]) //统计各层结点数
    {
        wide[i++]++;
        if (p->leftchild != NULL)
            get_width(p->leftchild, i, wide);
        if (p->rightchild != NULL)
            get_width(p->rightchild, i, wide);
    }
    /*
	TODO:统计并返回二叉树的宽度（本题不用使用递归）
	 */
    int get_max_width()
    {
        if(this->size==0) return 0;
        vector<int> num(this->size+1);
        queue<pair<binarytreenode*,int> > que;
        que.push(make_pair(root,1));
        while(!que.empty())
        {
            pair<binarytreenode*,int> st=que.front();
            que.pop();
            num[st.second]++;
            if(st.first->leftchild!=NULL) que.push(make_pair(st.first->leftchild,st.second+1));
            if(st.first->rightchild!=NULL) que.push(make_pair(st.first->rightchild,st.second+1));
        }
        int ans=*max_element(num.begin(),num.end());
        vector<int>().swap(num);
        return ans;
    }

    /*
	TODO: 计算并返回二叉树的最大值
	 */
    int get_max(binarytreenode* p)
    {
        if(p==NULL) return int((1<<31)+1);
        return max(p->get_data(),max(get_max(p->leftchild),get_max(p->rightchild)));
    }
    /*
	TODO:交换二叉树的左右孩子
	 */
    void change_children(binarytreenode* &p)
    {
        // 意思是交换所有的节点的左右孩子
        if(p==NULL) return;
        change_children(p->leftchild);
        change_children(p->rightchild);
        swap(p->leftchild,p->rightchild);
    }
    int find_father(binarytreenode* num, binarytreenode*& fa) //寻找父节点
    {
        binarytreenode* p = root;
        int flag = 0;
        while (p != NULL) {
            if (p->get_data() > num->get_data()) {
                fa = p;
                flag = 1;
                p = p->leftchild;
            } else if (p->get_data() <= num->get_data() && p != num) {
                fa = p;
                flag = 2;
                p = p->rightchild;
            } else {
                break;
            }
        }
        return flag;
    }
    /*
	TODO:删除叶节点，删除成功的时候输出打印cout << xxx << "删除成功!" << endl;其中xxx为叶子节点的值
	 */
    void del_leaf(binarytreenode*& p)
    {
        // 删除度为2的节点
        if(this->size==1) return;
        if(p==NULL) return;
        else
        {
            if(leaf(p)){
                cout << p->get_data() << "删除成功!" << endl;
                delete p;
                p=NULL;
                size--;
                return;
                // 删完直接返回，不然会产生新的
            }
            del_leaf(p->rightchild);
            del_leaf(p->leftchild);
        }
    }
    bool leaf(binarytreenode* pCur)
    {
        if (pCur->leftchild == NULL && pCur->rightchild == NULL)
            return true;
        return false;
    }
    /*
TODO:判断是否是完全二叉树，是则返回true，否则返回false。
 */
    bool wanquan(binarytreenode* pRoot) //是否完全二叉树
    {
        // 空树是完全二叉树
        queue<binarytreenode*> que;
        que.push(root);
        while(!que.empty())
        {
            binarytreenode* st=que.front();
            que.pop();
            if(st==NULL) break;//如果遇到空节点，但是还是有元素，则不是完全二叉树
            que.push(st->leftchild);
            que.push(st->rightchild);
        }
        while(!que.empty() and que.front()==NULL) que.pop();
        if(que.empty()) return true;
        else return false;
    }
};
int binarytree::times = 0;
int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    binarytree tree;
    tree.creat();
    tree.levelorder();
    cout << "度为1的结点个数: " << tree.degree1(tree.get_root()) << endl;
    cout << "度为2的结点个数: " << tree.degree2(tree.get_root()) << endl;
    cout << "度为0的结点个数: " << tree.degree0(tree.get_root()) << endl;
    cout << "二叉树高度: " << tree.get_height(tree.get_root()) << endl;
    cout << "二叉树宽度: " << tree.get_max_width() << endl;
    cout << "最大值:" << tree.get_max(tree.get_root()) << endl;

    cout << "删除叶节点:" << endl;
    binarytreenode* tmp=tree.get_root();
    tree.del_leaf(tmp);
    tree.levelorder();
    cout << "交换左右孩子. . ." << endl;
    tmp=tree.get_root();
    tree.change_children(tmp);
    tree.levelorder();
    if (tree.wanquan(tree.get_root()))
        cout << "该二叉树是完全二叉树" << endl;
    else
        cout << "该二叉树不是完全二叉树" << endl;
    return 0;
}
