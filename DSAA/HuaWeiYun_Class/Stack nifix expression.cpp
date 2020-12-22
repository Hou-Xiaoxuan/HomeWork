/**1.4-d,e 计算器：中缀表达式转换为后缀，并计算

	void infix_to_suffix();   //1.4-e  将中缀表达式转换为后缀表达
	void cal_suffix();        //1.4-d.	计算中缀表达式的值

**/

/*
*看了一下，根据toNum函数的实现，应该没有支持带小数点的char*的转换，那就默认输入中不存在小数点，即输入全部是int型
*没过样例，估计是需要考虑小数点啊……
*考虑了小数点，但是还没有过……不知道锅在哪里啊
*/
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

template <class T>
class stack {
private:
    int top; //栈顶
    int maxtop; //最大的栈顶值
    T* st; //堆栈元素数组
public:
    stack(int maxstacksize = 10)
    {
        maxtop = maxstacksize - 1;
        st = new T[maxstacksize];
        top = -1;
    }
    ~stack()
    {
        delete[] st;
    }
    bool isfull()
    {
        return top == maxtop;
    }
    bool isempty()
    {
        return top == -1;
    }
    bool add(const T& x)
    {
        if (isfull()) {
            cout << "no memory" << endl;
            return false;
        }
        top++;
        st[top] = x;
        return true;
    }
    T del()
    {
        if (isempty()) {
            cerr << "no element" << endl;
        }
        T x;
        x = st[top];
        top--;
        return x;
    }
    T returntop()
    {
        return st[top];
    }
    void clear()
    {
        top = -1;
    }
    void output()
    {
        if (isempty()) {
            cout << "栈是空的 " << endl;
        } else {
            for (int i = 0; i <= top; i++) {
                cout << st[i] << '\t';
            }
            cout << endl;
        }
    }
};

class Calculator {
public:
    string infix; //中缀表达式
    string postfix; //后缀表达式
    stack<double> poststack; //后缀栈
    Calculator(string str);
    ~Calculator()
    {
        poststack.clear();
    }
    void infix_to_suffix(); //1.4-e  将中缀表达式转换为后缀表达
    int inStack(char ch); //栈内优先级
    int outStack(char ch); //栈外优先级数
    void cal_suffix(); //1.4-d.	计算中缀表达式的值
    void print();
    bool Get_operands(double& left, double& right);
    bool isNumber(char ch);
    double returnnum(const char* c, int n); //返回字符对应的数
    float toNum(char num_ch); //数值型字符转换成数字
    void cal(char ch); //根据操作符 去栈中去取两个元素计算
};
Calculator::Calculator(string str)
{
    infix = str;
}

string format(string str) {	
    			
    for (int i = 0; i < str.length(); i++)
    {
		if (str[i] == '-')
			if (i == 0)
				str.insert(0, 1, '0');
			else if (str[i - 1] == '(')
				str.insert(i, 1, '0');
    }
	return str;

}
/*
TODO:1.4-e  将中缀表达式转换为后缀表达,假如中缀表达式为1+(2-3)*4+4/2，
则转换后为：1&2&3&-4&*+4&2&/+ [为了区分数字与数字，用&符号将数字进行隔开，当然也可自行选择其他方式来表达]
 */
void Calculator::infix_to_suffix()
{
        /*转后缀(这一部分有点迷……)*/
        stack<char> sta(100);
        for(int i=0;i<infix.size();i++)
        {
            // 只有操作符和运算数和括号和小数点
            if(!isNumber(infix[i]) and infix[i]!='.')//如果是操作符/括号
            {
                char ch=infix[i];//临时变量
                if(ch=='('){
                   sta.add(ch);
                }
                else if(ch==')')
                {
                    while(!sta.isempty() and sta.returntop()!='('){
                        postfix.push_back(sta.returntop());
                        sta.del();
                    }
                    sta.del();//删掉(
                }
                else
                {
                    if(sta.isempty() or inStack(ch)>inStack(sta.returntop())) sta.add(ch);
                    else
                    {
                        while(!sta.isempty() and inStack(ch)<=inStack(sta.returntop())){
                            postfix.push_back(sta.returntop());
                            sta.del();
                        }
                        sta.add(ch);
                    }
                    
                }
            }
            else//如果遭遇操作数
            {
                while(i<infix.size() and  (isNumber(infix[i]) or infix[i]=='.')){
                    postfix.push_back(infix[i]);
                    i++;
                }
                i--;
                postfix.push_back('&');
            }
        }
        while(!sta.isempty()){
            postfix.push_back(sta.returntop());
            sta.del();
        }
}

/*
TODO: 1.4-d.计算中缀表达式的值，比如：假如中缀表达式为1+(2-3)*4+4/2，则根据四则混合运算优先级计算，它的结果为-1.
如果输入的中缀表达式非法，则打印cout << "重新输入 并检查表达式的合法性" << endl;，然后exit(1)结束执行。
*/
void Calculator::cal_suffix()
{
    infix_to_suffix(); //将中缀表达式转换为后缀表达式

        /*开始计算*/
        for(int i=0;i<postfix.size();i++)
        {
            if(!isNumber(postfix[i]) and postfix[i]!='&')//如果是操作符
            {
               cal(postfix[i]);
            }
            else//数字/分隔符/.
            {
                if(postfix[i]=='&') i++;
                int j=i;
                while(isNumber(postfix[++j]));
                double int_part=returnnum(postfix.substr(i,j-i).data(),j-i);
                if(postfix[j]=='&'){//如果没有小数部分
                    i=j;
                    poststack.add(int_part);
                    continue;
                }
                else if(postfix[j]=='.')
                {
                    // 获得小数部分
                   
                    j++;
                    i=j;
                    while(isNumber(postfix[++j]));
                    double frac_part=returnnum(postfix.substr(i,j-i).data(),j-i);
                    while(frac_part>1) frac_part/=10;
                    i=j;
                    poststack.add(int_part+frac_part);
                }

            }
        }
        /*计算结束*/
}

bool Calculator::Get_operands(double& left, double& right)
{
    if (poststack.isempty()) {
        cout << "缺少右操作数" << endl;
        return false;
    }
    right = poststack.del(); //取右操作数
    if (poststack.isempty()) {
        cout << "缺少左操作数" << endl;
        return false;
    }
    left = poststack.del(); //取出左操作数
    return true; //返回true
}
double Calculator::returnnum(const char* c, int n) //返回字符对应的数,n表示个十百千位
{
    int l = n;
    double num = 0;
    double m = 1;
    for (int i = l - 1; i >= 0; i--) {
        num += toNum(c[i]) * m;
        m *= 10;
    }
    return num;
}

int Calculator::inStack(char c)
{
    switch (c) {
    case '#':
        return 0;
        break;
    case '(':
        return 1;
        break;
    case '*':
    case '/':
    case '%':
        return 5;
        break;
    case '+':
    case '-':
        return 3;
        break;
    case ')':
        return 6;
        break;
    }
}
//返回操作符的栈外优先级
int Calculator::outStack(char c)
{
    switch (c) {
    case '#':
        return 0;
        break;
    case '(':
        return 6;
        break;
    case '*':
    case '/':
    case '%':
        return 4;
        break;
    case '+':
    case '-':
        return 2;
        break;
    case ')':
        return 1;
        break;
    }
}
//判断是否为操作数
bool Calculator::isNumber(char ch)
{
    if (48 <= ch && ch <= 57)
        return true;
    else
        return false;
}

//数值型字符转换为数字
float Calculator::toNum(char num_ch)
{
    switch (num_ch) {
    case '0':
        return 0;
        break;
    case '1':
        return 1;
        break;
    case '2':
        return 2;
        break;
    case '3':
        return 3;
        break;
    case '4':
        return 4;
        break;
    case '5':
        return 5;
        break;
    case '6':
        return 6;
        break;
    case '7':
        return 7;
        break;
    case '8':
        return 8;
        break;
    case '9':
        return 9;
        break;
    }
}
//根据操作符 去栈中去取两个元素计算
void Calculator::cal(char cp)
{
    double left, right, value;
    if (Get_operands(left, right)) {
        switch (cp) {
        case '+': {
            value = left + right;
            poststack.add(value); //相加后结果压栈
            break;
        }
        case '-': {
            value = left - right;
            poststack.add(value); //相减 压栈
            break;
        }
        case '*': {
            value = left * right;
            poststack.add(value); //相乘 压栈
            break;
        }
        case '/':
            if (right == 0) {
                cout << "/ 操作符 右操作数为0" << endl;
                exit(1);
            } else {
                value = left / right;
                poststack.add(value); //相除 压栈
            }
            break;
        }
    }
}
//保留并输出最后结果
void Calculator::print()
{
    cout << "表达式计算结果:" << endl;
    cout << poststack.returntop() << endl;
}
int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    string str;
    cout << "输入一个中缀表达式：" << endl;
    cin >> str;
    // ！！！！这里的顺序反了，需要先格式化再初始化
    str = format(str);
    Calculator calculator(str);
    calculator.cal_suffix();
    calculator.print(); //显示结果
    return 0;
}
