'''
Author: LinXuan
Date: 2021-11-09 20:32:16
Description: 
LastEditors: LinXuan
LastEditTime: 2021-12-06 22:33:13
FilePath: /Python Code/src/task_CompilationPrinciple/task_5_.py
'''
import copy
import re
import sys
sys.path.append("/run/media/linxuan/Data/Python Code/Python Code/src")
sys.path.append(r"D:\Python Code\Python Code\src")
from task_CompilationPrinciple.task_3_all import isTerminator


# 构造分析表


def get_table():
    # 手动构造
    row = {
        "+": None,
        "*": None,
        "(": None,
        ")": None,
        "id": None,
        "$": None,
        "E": None,
        "F": None,
        "T": None
    }
    table = [copy.deepcopy(row) for i in range(12)]
    table[0]['('] = 's4'
    table[0]['id'] = 's5'
    table[0]['E'] = 1
    table[0]['F'] = 3
    table[0]['T'] = 2

    table[1]['+'] = 's6'
    table[1]['$'] = 'acc'

    table[2]['*'] = 's7'
    table[2]['+'] = table[2][')'] = table[2]['$'] = 'r2'

    table[3]['+'] = table[3]['*'] = table[3][')'] = table[3]['$'] = 'r4'

    table[4]['('] = 's4'
    table[4]['id'] = 's5'
    table[4]['E'] = 8
    table[4]['F'] = 3
    table[4]['T'] = 2

    table[5]['+'] = table[5]['*'] = table[5][')'] = table[5]['$'] = 'r6'

    table[6]['('] = 's4'
    table[6]['id'] = 's5'
    table[6]['F'] = 3
    table[6]['T'] = 9

    table[7]['('] = 's4'
    table[7]['id'] = 's5'
    table[7]['F'] = 10

    table[8]['+'] = 's6'
    table[8][')'] = 's11'

    table[9]['+'] = table[9]['*'] = table[9][')'] = table[9]['$'] = 'r1'

    table[10]['+'] = table[10]['*'] = table[10][')'] = table[10]['$'] = 'r3'

    table[11]['+'] = table[11]['*'] = table[11][')'] = table[11]['$'] = 'r5'

    return table


def LR_analysis(sentence: str, table: dict, grammar="", begin="E"):
    sentence = sentence.replace("#", "$")  # 替换结束符
    # 拆分输入为stack
    input: list = list(re.findall(r"[a-z]+|[+*()$]", sentence))
    input.reverse()

    stack = [0]
    print("-------------Start-------------")
    while len(stack) > 0:
        if type(stack[-1]) == int:
            state = stack[-1]
            ch = input[-1]
            action = table[state][ch]
            if action == None:
                print("error: ", end="")
                A = None
                while len(stack) > 0:
                    could = [i for i in ['E', 'F', 'T']
                             if table[state][i] != None]
                    if len(could) > 0:
                        A = could[0]
                        break
                    else:
                        print(f"弹出状态{stack.pop()}")
                        print(f"弹出符号{stack.pop()}")
                if A == None:
                    break
                print(f"选择A={A}", end=";")
                state = stack[-1]
                stack.append(A)
                print(f"将{A}入栈", end=";")
                stack.append(table[state][A])
                print(f"将状态{table[state][A]}入栈", end=";")
                print("")
                # end
            else:
                # 执行动作
                if action[0] == 's':
                    # 移进
                    print("移进")
                    input.pop()
                    stack.append(ch)
                    stack.append(int(action[1:]))
                elif action[0] == 'r':
                    # 归约，不会出错
                    print(
                        f"按照{grammar[action][0]} -> {grammar[action][1]}进行归约")
                    stack = stack[0:len(grammar[action][1]) * (-2)]
                    state = stack[-1]
                    stack.append(grammar[action][0])
                    stack.append(table[state][grammar[action][0]])
                elif action == 'acc':
                    print("接受")
                    break

    print("结束")


grammar = {
    "r1": ("E", ["E", "+", "T"]),
    "r2": ("E", ["T"]),
    "r3": ("T", ["T", "*", "F"]),
    "r4": ("T", ["F"]),
    "r5": ("F", ["(", "E", ")"]),
    "r6": ("F", ["id"]),
}
sentence = "id*id++id#"
table = get_table()
LR_analysis(sentence=sentence, table=table, grammar=grammar)
