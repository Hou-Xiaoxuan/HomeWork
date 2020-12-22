/**1.4-d,e ����������׺���ʽת��Ϊ��׺��������

	void infix_to_suffix();   //1.4-e  ����׺���ʽת��Ϊ��׺���
	void cal_suffix();        //1.4-d.	������׺���ʽ��ֵ

**/

/*
*����һ�£�����toNum������ʵ�֣�Ӧ��û��֧�ִ�С�����char*��ת�����Ǿ�Ĭ�������в�����С���㣬������ȫ����int��
*û����������������Ҫ����С���㰡����
*������С���㣬���ǻ�û�й�������֪���������ﰡ
*/
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

template <class T>
class stack {
private:
    int top; //ջ��
    int maxtop; //����ջ��ֵ
    T* st; //��ջԪ������
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
            cout << "ջ�ǿյ� " << endl;
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
    string infix; //��׺���ʽ
    string postfix; //��׺���ʽ
    stack<double> poststack; //��׺ջ
    Calculator(string str);
    ~Calculator()
    {
        poststack.clear();
    }
    void infix_to_suffix(); //1.4-e  ����׺���ʽת��Ϊ��׺���
    int inStack(char ch); //ջ�����ȼ�
    int outStack(char ch); //ջ�����ȼ���
    void cal_suffix(); //1.4-d.	������׺���ʽ��ֵ
    void print();
    bool Get_operands(double& left, double& right);
    bool isNumber(char ch);
    double returnnum(const char* c, int n); //�����ַ���Ӧ����
    float toNum(char num_ch); //��ֵ���ַ�ת��������
    void cal(char ch); //���ݲ����� ȥջ��ȥȡ����Ԫ�ؼ���
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
TODO:1.4-e  ����׺���ʽת��Ϊ��׺���,������׺���ʽΪ1+(2-3)*4+4/2��
��ת����Ϊ��1&2&3&-4&*+4&2&/+ [Ϊ���������������֣���&���Ž����ֽ��и�������ȻҲ������ѡ��������ʽ�����]
 */
void Calculator::infix_to_suffix()
{
        /*ת��׺(��һ�����е��ԡ���)*/
        stack<char> sta(100);
        for(int i=0;i<infix.size();i++)
        {
            // ֻ�в������������������ź�С����
            if(!isNumber(infix[i]) and infix[i]!='.')//����ǲ�����/����
            {
                char ch=infix[i];//��ʱ����
                if(ch=='('){
                   sta.add(ch);
                }
                else if(ch==')')
                {
                    while(!sta.isempty() and sta.returntop()!='('){
                        postfix.push_back(sta.returntop());
                        sta.del();
                    }
                    sta.del();//ɾ��(
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
            else//�������������
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
TODO: 1.4-d.������׺���ʽ��ֵ�����磺������׺���ʽΪ1+(2-3)*4+4/2��������������������ȼ����㣬���Ľ��Ϊ-1.
����������׺���ʽ�Ƿ������ӡcout << "�������� �������ʽ�ĺϷ���" << endl;��Ȼ��exit(1)����ִ�С�
*/
void Calculator::cal_suffix()
{
    infix_to_suffix(); //����׺���ʽת��Ϊ��׺���ʽ

        /*��ʼ����*/
        for(int i=0;i<postfix.size();i++)
        {
            if(!isNumber(postfix[i]) and postfix[i]!='&')//����ǲ�����
            {
               cal(postfix[i]);
            }
            else//����/�ָ���/.
            {
                if(postfix[i]=='&') i++;
                int j=i;
                while(isNumber(postfix[++j]));
                double int_part=returnnum(postfix.substr(i,j-i).data(),j-i);
                if(postfix[j]=='&'){//���û��С������
                    i=j;
                    poststack.add(int_part);
                    continue;
                }
                else if(postfix[j]=='.')
                {
                    // ���С������
                   
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
        /*�������*/
}

bool Calculator::Get_operands(double& left, double& right)
{
    if (poststack.isempty()) {
        cout << "ȱ���Ҳ�����" << endl;
        return false;
    }
    right = poststack.del(); //ȡ�Ҳ�����
    if (poststack.isempty()) {
        cout << "ȱ���������" << endl;
        return false;
    }
    left = poststack.del(); //ȡ���������
    return true; //����true
}
double Calculator::returnnum(const char* c, int n) //�����ַ���Ӧ����,n��ʾ��ʮ��ǧλ
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
//���ز�������ջ�����ȼ�
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
//�ж��Ƿ�Ϊ������
bool Calculator::isNumber(char ch)
{
    if (48 <= ch && ch <= 57)
        return true;
    else
        return false;
}

//��ֵ���ַ�ת��Ϊ����
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
//���ݲ����� ȥջ��ȥȡ����Ԫ�ؼ���
void Calculator::cal(char cp)
{
    double left, right, value;
    if (Get_operands(left, right)) {
        switch (cp) {
        case '+': {
            value = left + right;
            poststack.add(value); //��Ӻ���ѹջ
            break;
        }
        case '-': {
            value = left - right;
            poststack.add(value); //��� ѹջ
            break;
        }
        case '*': {
            value = left * right;
            poststack.add(value); //��� ѹջ
            break;
        }
        case '/':
            if (right == 0) {
                cout << "/ ������ �Ҳ�����Ϊ0" << endl;
                exit(1);
            } else {
                value = left / right;
                poststack.add(value); //��� ѹջ
            }
            break;
        }
    }
}
//��������������
void Calculator::print()
{
    cout << "���ʽ������:" << endl;
    cout << poststack.returntop() << endl;
}
int main()
{
    #ifdef _DEBUG
    freopen("D:\\CODE\\c++\\data.in","r",stdin);
    freopen("D:\\CODE\\c++\\data.out","w",stdout);
    #endif // DEBUG

    string str;
    cout << "����һ����׺���ʽ��" << endl;
    cin >> str;
    // �������������˳���ˣ���Ҫ�ȸ�ʽ���ٳ�ʼ��
    str = format(str);
    Calculator calculator(str);
    calculator.cal_suffix();
    calculator.print(); //��ʾ���
    return 0;
}
