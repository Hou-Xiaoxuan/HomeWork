#include<iostream>
#include<unordered_map>
#include<vector>
#include<queue>
#include<string>
#include<stack>
using namespace std;

/*
*构造一棵霍夫曼树
*平衡树暂时被我抛弃了……
*/
class Huffman
{
private:
    /*Huffman树的树节点*/
    struct Node
    {
        Node(size_t val=0,bool is=false):
            value(val),is_char(is),left_child(nullptr),right_child(nullptr){};
        size_t value;//权重
        bool is_char;//节点是否为字符
        char charactor;//节点是否代表字符
        char coded_value;//向改节点走编码的变化(0 or 1)
        Node *left_child,*right_child;
        bool operator<(const Node& st){
            return value<st.value;
        }
    };
    /*用于指针的优先队列比较参数*/
    struct cmp_Node
    {
        bool operator() (const Node*a,const Node*b){
            return a->value > b->value;//最小值优先,不带参数默认最大值优先
        }
    };
    /*生成的译码表*/
    unordered_map<char,string> Code_Table;
    unordered_map<string,char> Decod_table;
    Node *Root;
public:
    Huffman(/* args */)
    {
        Root = nullptr;
    }
    Huffman(const string& text)
    {
        Root = nullptr;
        build(text);
        get_table();
    }
    
    void build(const string& text)
    {
        // 统计字母个数
        vector<int> number_of_char(257,0);
        for(const char i:text)
            number_of_char[size_t(i)]++;
        priority_queue<Node*,vector<Node*>,cmp_Node> que;
        for(size_t i=0;i<number_of_char.size();i++)
        {
            if(number_of_char[i]==0) continue;
            else
            {
                Node *new_node = new Node(number_of_char[i],true);
                new_node->charactor=char(i);
                que.push(new_node);
            }
        }
        // 构造霍夫曼树
        while(que.size()>1)
        {
            auto left = que.top();
            que.pop();
            left->coded_value='0';
            auto right = que.top();
            que.pop();
            right->coded_value = '1';
            Node* new_node = new Node(left->value+right->value,false);
            new_node->left_child=left;
            new_node->right_child=right;
            que.push(new_node);
        }
        Root = que.top();
        que.pop();
    }
    
    void get_table()
    {
        /*将树上的信息转换成译码表*/
        stack<pair<Node*,string> > sta;
        sta.push(make_pair(Root,""));
        while(!sta.empty())
        {
            auto st = sta.top();
            sta.pop();
            if(st.first->is_char==true)//该节点是字符
            {
                Code_Table[st.first->charactor]=st.second;
                Decod_table[st.second]=st.first->charactor;
                continue;
            }
            if(st.first->left_child!=nullptr){
                sta.push(make_pair(st.first->left_child,st.second+'0'));
            }
            if(st.first->right_child!=nullptr){
                sta.push(make_pair(st.first->right_child,st.second+'1'));
            }
        }
    }

    string code(const string&Text)
    {
        /*编码*/
        string Code;
        for(auto i:Text)
            Code+=Code_Table[i];
        return Code;
    }
    string decode(const string& Code)
    {
        string Text;
        string tmp_str;
        for(size_t i=0;i<Code.size();i++)
        {
            tmp_str.push_back(Code[i]);
            if(Decod_table.count(tmp_str)!=0){
                Text.push_back(Decod_table[tmp_str]);
                tmp_str.clear();
            }
        }
        return Text;
    }
    ~Huffman()
    {
        stack<Node*> sta;
        if(Root!=nullptr) sta.push(Root);
        while(!sta.empty())
        {
            Node*st=sta.top();
            sta.pop();
            if(st->left_child!=nullptr) sta.push(st->left_child);
            if(st->right_child!=nullptr) sta.push(st->right_child);
            delete st;
        }
    }
};


int main()
{
    Huffman Huff;
    string text = "aaaaaaaaaa\nbbbb  bbbbbbbcccccddddddedfa";
    Huff.build(text);
    Huff.get_table();
    string code=Huff.code(text);
    cout<<Huff.decode(code)<<endl;
    return 0;
}