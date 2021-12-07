'''
Author: 侯富中
Date: 2021-09-15 11:06:09
Description: 对循环语句和条件判断语句编写词法分析编译程序，只能通过一遍扫描完成。
LastEditors: LinXuan
LastEditTime: 2021-10-12 15:56:29
FilePath: /Python Code/src/task_CompilationPrinciple/1.1词法分析.py
'''

import re
import sys
# 词法记号
keyword = dict({
    r"for": 1,
    r"if": 2,
    r"then": 3,
    r"else": 4,
    r"while": 5,
    r"do": 6,
    r"until": 29,
    r"int": 30,
    r"input": 31,
    r"output": 32
})
simbal = dict({
    r"#": 0,
    r"+": 11,
    r"-": 14,
    r"*": 15,
    r"/": 16,
    r":": 17,
    r":=": 18,
    r"<": 20,
    r"<>": 21,
    r"<=": 22,
    r">": 23,
    r">=": 24,
    r"=": 25,
    r";": 26,
    r"(": 27,
    r")": 28,
})
# simbal的字符串顺序有要求。
simbal_list = ["#", "\+", "-", "\*", "/", ":=", ":",
               "<>", "<=", "<", ">=", ">", "=", ";", "\(", "\)"]
# 正则列表
keyword_re = [re.compile(s) for s in keyword.keys()]
simbal_re = [re.compile(s) for s in simbal_list]
identifier_re = re.compile(r"[a-z][a-z0-9]*")  # 10
# 可以通过扩展num_re来实现匹配浮点数和科学技术法
num_re = re.compile(r"\d+")  # 11


def GetNextId(text):
    """
        从接受到的字符串开头进行匹配，返回匹配到的id
    """
    text = str(text)

    # 优先匹配关键字
    for rekey in keyword_re:
        id = rekey.match(text)
        if(id != None):
            id = id.group()
            return id, keyword[id]

    # 匹配运算符
    for resimbal in simbal_re:
        id = resimbal.match(text)
        if id != None:
            id = id.group()
            return id, simbal[id]

    # 匹配标识符
    id = identifier_re.match(text)
    if id != None:
        id = id.group()
        return id, 10

    id = num_re.match(text)
    if id != None:
        id = id.group()
        return id, 11
    return None, -1


def LexicalAnalysisasis(text):
    ans = []
    text = str(text)
    while len(text) > 0:
        while text.startswith(" ") or text.startswith("\r") or text.startswith("\n"):
            text = text[1:]
        id, token = GetNextId(text)
        if id != None:
            ans.append((token, id))
        else:
            if text == "":
                print("扫描完成")
            else:
                print("语法错误，出现非法符号")
        text = text[len(id):]
    return ans


def main():
    text = "int x:=5;  if (x>0)  then  x:=2*x+1/3;  else  x:=2/x;  #"
    if len(sys.argv) == 2:
        try:
            text = sys.argv[1]
        except IndexError:
            print("参数个数错误, 应为 pthon 1.1词法分析.py text")
    ans = LexicalAnalysisasis(text)
    for tp in ans:
        print(tp, end=" ")
    print("")
    pass


if __name__ == "__main__":
    main()
