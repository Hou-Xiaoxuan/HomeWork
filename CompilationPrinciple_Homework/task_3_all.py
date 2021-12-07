'''
Author: LinXuan
Date: 2021-10-12 15:33:58
Description: 编译上机作业，计算first、follow集合、消除左递归
LastEditors: LinXuan
LastEditTime: 2021-11-09 18:08:28
FilePath: /Python Code/src/task_CompilationPrinciple/task_3_all.py
'''
'''
    1. 由于字符集问题，推导符号只能是->。且，空使用符号&来表示
    2. 在消除左递归的过程中，为了防止字符重复，引入大写字母+`！`，如`A!`作为终结符
'''
import copy
import re


def read_grammar(grammar_text: list) -> dict:
    """拆分识别终结符和非终结符，返回识别好的终结符、非终结符和文法式"""
    # grammar_list = [""]
    ret = dict()
    for grammar in grammar_text:
        assert grammar.count("->") == 1, "文法表达式非法"
        part_a, part_b = grammar.split("->")
        left = part_a.strip()
        part_b = part_b.split("|")
        right = []
        re_stmt = re.compile(r"[A-Z]'?|[a-z]+|[(+*&)]")
        for part in part_b:
            right.append(list(re_stmt.findall(part)))
        ret[left] = right
    return ret


def isTerminator(key) -> bool:
    """判断是否为终结符"""
    return re.match(r"[A-Z]'?!?", key) == None


def get_first_set(grammar: dict) -> dict:
    """生成文法first集的迭代求法"""
    First = dict()
    grammar = dict(grammar)
    # 初始化终结符情况
    for key, value in zip(grammar.keys(), grammar.values()):
        First[key] = set()  # 初始化
        # 遍历每一个可能的文法串, 判断第一个符号是否为终结符
        for stmt in value:
            # 规则(1)、(2)
            if isTerminator(stmt[0]):
                First[key].add(stmt[0])

    # 开始迭代，直至无变化发生
    did = True  # 是否有变化
    while did == True:
        did = False
        for key, value in zip(grammar.keys(), grammar.values()):
            for stmt in value:
                flag = True  # 支付能包含 空
                for item in stmt:
                    if isTerminator(item):
                        flag = False
                        break  # 退出
                    if First[item] - set("&") - First[key] != set():
                        did = True
                    # 规则(3)
                    First[key] = First[key].union(First[item] - set("&"))

                    if "&" not in First[item]:
                        flag = False
                        break

                # 规则(4)
                if flag == True and "&" not in First[key]:
                    First[key].add("&")
                    did = True
            pass
        pass
    return First


def get_follow_set(grammar: dict, First: dict, start: str) -> dict:
    '''
        @grammar: 文法
        @First: 计算好的first集合
        @start: 文法的开始符号
        返回文法的follow集
    计算规则：
        1. 对文法开始符号 S, 置 $ 于 FOLLOW(S) 中。
        2. 若有 A->BC ,则将 FIRST(C)\{&} 加入 FOLLOW(B) 中。
        3. 若 A->AB 或 A-> ABC, 且 C->& (即&属于FIRST(C) ) , 则将 FOLLOW(A) 加入 FOLLOW(B)
    '''
    Follow = dict()
    # 初始化
    for key in First.keys():
        Follow[key] = set()
    Follow[start] = set("$")  # 开始符号，规则(1)

    did = True
    while did:
        did = False
        for key, value in zip(grammar.keys(), grammar.values()):
            for stmt in value:  # 遍历每个可能
                # 规则(2)
                for i in range(len(stmt) - 1):
                    if isTerminator(stmt[i]) == True:
                        continue
                    else:
                        update: set = Follow[stmt[i]]

                        if isTerminator(stmt[i + 1]):
                            update = update.union(set(stmt[i + 1]))
                        else:
                            update = update.union(First[stmt[i + 1]] - set("&"))
                        # 判断是否发生更新
                        if Follow[stmt[i]] != update:
                            Follow[stmt[i]] = update
                            did = True
                # 规则(3)
                for i in range(len(stmt) - 1, -1, -1):
                    if isTerminator(stmt[i]):
                        break
                    else:
                        update: set = Follow[stmt[i]]
                        update = update.union(Follow[key])
                        # 判定是否发生更新
                        if update != Follow[stmt[i]]:
                            did = True
                            Follow[stmt[i]] = update
                        if("&" not in First[stmt[i]]):
                            break

    return Follow


def eliminate_left_recursion(grammar: dict) -> dict:
    '''
    消除左递归
    '''
    # 间接左递归转直接左递归
    order = dict()
    for ord, key in zip(range(0, len(grammar)), grammar.keys()):
        order[key] = ord

    while True:
        new_grammar: dict = copy.deepcopy(grammar)

        for key, value in zip(grammar.keys(), grammar.values()):  # 文法式
            for stmt in value:
                # 判断stmt的第一个符号，是否需要进行替换
                if isTerminator(stmt[0]):  # 遇到终结符
                    break
                if order[key] < order[stmt[0]]:
                    back = stmt[1:]  # 本语句的后半部分
                    rep_stmt = [rep + back for rep in grammar[stmt[0]]]
                    new_grammar[key].remove(stmt)
                    new_grammar[key].extend(rep_stmt)
                break

        if grammar.__eq__(new_grammar):
            break
        grammar = new_grammar

    # 消除直接左递归
    did = True
    while did == True:
        did = False
        for key in grammar.keys():  # 文法式
            value = grammar[key]
            alpha = []
            beta = []
            for stmt in value:
                if stmt[0] == key:
                    alpha.append(stmt[1:])
                else:
                    beta.append(stmt)
            if alpha.__len__() > 0:
                # 展开消除工作
                assert len(beta) > 0, "死循环，表达式不合法"
                did = True
                grammar[key] = [ite + list([key + "!"]) for ite in beta]
                grammar[key + "!"] = [ite + list([key + "!"]) for ite in alpha]
                grammar[key + "!"].append(["&"])
                order[key + "!"] = order[key]  # 继承顺序
                break  # dict改变了，不能继续迭代，必须退出

    # 重新排列一下顺序
    new_grammar = {}
    for key in sorted(grammar.keys(), key=lambda x: (order[x], x)):
        new_grammar[key] = grammar[key]
    return new_grammar


def show_grammar(grammar: dict):
    for key, value in zip(grammar.keys(), grammar.values()):
        output = key + " -> "
        for stmt in value:
            for item in stmt:
                output += item
            if(stmt != value[-1]):
                output += " | "
        print(output)
    pass


def main():
    grammar_text = [
        # r"E -> TE'",
        # r"E' -> + TE' | &",
        # r"T -> FT'",
        # r"T' -> * FT' | &",
        # r"F -> (E) | id"

        # r"A -> BC",
        # r"B -> AD | C",
        # r"C -> c",
        # r"D -> d"
        # r"E->E+T|T",
        # r"T->TF|F",
        # r"F->F*|a|b"
        "E -> E + T | T",
        "T -> T * F | F",
        "F -> (E) | id",
    ]
    grammar = read_grammar(grammar_text)
    print("初始的文法式: ")
    show_grammar(grammar)

    # grammar = eliminate_left_recursion(grammar)
    # print("消除一切左递归后的文法式: ")
    # show_grammar(grammar)

    First = get_first_set(grammar)
    print("求得First集，如下: ")
    for item in First.items():
        print(item[0], "\t", item[1])

    Follow = get_follow_set(grammar, First, grammar_text[0][0])
    print("求得Follow集如下: ")
    for item in Follow.items():
        print(item[0], "\t", item[1])


if __name__ == "__main__":
    main()
