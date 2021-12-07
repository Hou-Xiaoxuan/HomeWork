'''
Author: LinXuan
Date: 2021-10-26 12:53:17
Description:
LastEditors: LinXuan
LastEditTime: 2021-11-22 21:15:58
FilePath: /Python Code/src/task_CompilationPrinciple/task_4_.py
'''
'''
    1. 同步记号集简单采用Follow集合
    2. Follow集和First集的计算在上次上机已经实现
'''
import sys
from re import escape

sys.path.append("src")
from task_CompilationPrinciple.task_3_all import *


def init_table(grammar: dict):
    """
        初始化分析表，定义好需要的表项
    """
    table = dict()
    # 得到所有的终结符和非终结符
    Terminator = set()
    NonTerminator = set()
    for key in grammar.keys():
        NonTerminator.add(key)
        for stmt in grammar[key]:
            for item in stmt:
                if isTerminator(item):
                    Terminator.add(item)
                else:
                    NonTerminator.add(item)
    Terminator.add('$')  # 终结符
    Terminator.remove('&')  # 取出空
    row = dict()
    for item in Terminator:
        row.update({item: ""})
    for item in NonTerminator:
        table.update({item: copy.deepcopy(row)})  # 必须复制，否则指向同一个对象

    # # debug,规定顺序
    # row = {
    #     "id": "",
    #     "+": "",
    #     "*": "",
    #     "(": "",
    #     ")": "",
    #     "$": ""
    # }
    # table = {
    #     "E": copy.deepcopy(row),
    #     "E'": copy.deepcopy(row),
    #     "T": copy.deepcopy(row),
    #     "T'": copy.deepcopy(row),
    #     "F": copy.deepcopy(row),
    # }
    return table


def get_first(stmt: list, First: dict):
    # 得到一个文法片段的first集合
    rt = set()
    for item in stmt:
        if isTerminator(item):
            rt.add(item)
            break
        else:
            rt = set.union(rt, First[item])
            if "&" not in First[item]:
                break
    return rt


def to_string(key: str, stmt: list):
    rt = key + " ->"
    for item in stmt:
        rt += " " + item
    return rt


def generate_table(grammar: dict, First: dict, Follow: dict) -> dict:
    table = init_table(grammar)
    for key in grammar.keys():
        for stmt in grammar[key]:
            add = get_first(stmt, First)  # 一段文法式的first集合
            sentence = to_string(key, stmt)  # 加入的表达式语句
            if '&' in add:
                add.remove('&')
                add = set.union(add, Follow[key])
            print(add, "增加了", sentence)
            for item in add:
                assert table[key][item] == "", "产生二义， 无法生成正确的分析表"
                table[key][item] = sentence

    return table


def add_synch(table: dict, Follow: dict):
    for key in table.keys():
        for item in Follow[key]:
            if table[key][item] == "":
                # print(key, item, "synch")
                table[key][item] = "synch"
    pass


def analyze(sentence: str, table: dict, start):
    sentence = sentence.replace("#", "$")  # 替换结束符
    # 拆分输入为stack
    input: list = list(re.findall(r"[a-z]+|[+*()$]", sentence))
    input.reverse()

    stack = ["$"]
    stack.append(start)  # 放入初始符号
    print("-------------Start-------------")
    while(stack[-1] != "$"):
        key = stack[-1]
        symbal = input[-1]
        if isTerminator(key):  # 栈顶是终结符
            if key == symbal:
                print(f"匹配 {key}")
                stack.pop()
                input.pop()
            else:
                print(f"出错；弹出stack中的{key}")
                stack.pop()
            pass
        else:
            if table[key][symbal] == "":
                print(f"Error 跳过input中的{symbal}")
                input.pop()
            elif table[key][symbal] == "synch":
                print(f"Error {symbal}是{key}的同步记号，无需跳过；{key}被弹出；")
                stack.pop()
            else:
                print(f"输出 {table[key][symbal]}")
                stack.pop()
                right: list = read_grammar([table[key][symbal]])[key][0]  # 前置的类型有问题，这里需要别扭的转换
                right.reverse()
                while "&" in right:
                    right.remove("&")
                stack += right
    print("-------------End---------------")

    pass


if __name__ == "__main__":
    grammar_text = [
        r"E -> TE'",
        r"E' -> + TE' | &",
        r"T -> FT'",
        r"T' -> * FT' | &",
        r"F -> (E) | id"
    ]
    grammar = read_grammar(grammar_text)  # 解析文法式
    First = get_first_set(grammar)  # 求first集
    Follow = get_follow_set(grammar, First, start="E")  # 求follow集
    table = generate_table(grammar, First, Follow)  # 生成分析表
    add_synch(table, Follow)  # 添加同步符号
    sentence = "+id*+id#"
    analyze(sentence, table, start="E")  # 进行解析
