'''
Author: xv_rong
Date: 2021-11-30 23:25:21
LastEditors: LinXuan
LastEditTime: 2021-12-06 21:54:34
Description:
'''

import copy
import re


origin_gramars = {
    r'0': r'L->E',
    r'1': r'E->E+T',
    r'2': r'E->E-T',
    r'3': r'E->T',
    r'4': r'T->T*F',
    r'5': r'T->T/F',
    r'6': r'T->F',
    r'7': r'F->(E)',
    r'8': r'F->id'
}


print('文法：')
for item in origin_gramars.items():
    print(item[1])
print()

# 词法分析
token_specification = [
    ('ID',
     r'((((\d)+\.(\d)*|(\d)*\.(\d)+)|((\d)+))[Ee][+-]?(\d)+)|((?<![Ee])(\d)+\.(\d)*|(\d)*\.(\d)+(?![Ee]))|((?<![Ee])(\d)+(?![Ee]))'),
    ('OP', r'[()+\-*/]'),
    ('BLANK', r'[ \n\t]+'),
    ('MISMATCH', r'.')
]

tok_regex = '|'.join('(?P<%s>%s)' % pair for pair in token_specification)
lexical_result: list[list[str, str]] = []


def tokenize(code):
    tmp = ''
    for mo in re.finditer(tok_regex, code):
        token = mo.lastgroup
        value = mo.group()

        if token == 'BLANK':
            continue
        elif token == 'ID':
            token = 'id'
            value = value
        elif token == 'OP':
            token = value
            value = ''
        elif token == 'MISMATCH':
            raise RuntimeError(f'{value!r} unexpected')

        # 特殊处理 单目+-号

        if token in '+-':
            if not (len(lexical_result) != 0 and (lexical_result[-1][0] == ')' or lexical_result[-1][0] == 'id')):
                tmp += token
                continue

        lexical_result.append([token, tmp + value])
        tmp = ''

    for item in tmp:
        lexical_result.append([item, ''])


# 语法分析
def isterminator(s):
    terminator = [r'*', r'+', r'-', r'/', r'(', r')', r'id', r'$']
    return s in terminator or s.islower()


def isempty(s):
    empty = 'ε'
    return s == empty


def split_symbol(s: str):
    res = []
    i = 0
    while i < len(s):
        c = s[i]
        if isterminator(s[i]) or isempty(s[i]):
            if c == 'i' and i < len(s) - 1 and s[i + 1] == 'd':
                c += s[i + 1]
                i += 1
        else:
            if i < len(s) - 1 and s[i + 1] == '\'':
                c += s[i + 1]
                i += 1
        res.append(c)
        i += 1
    return res


class Gramar:
    def __init__(self, left, right, id):
        self.left: str = left
        self.right: list[str] = right
        self.id: str = id


# 分隔文法
gramars_by_id: dict[int, Gramar] = {}
gramars_by_left: dict[str, list[Gramar]] = {}

for key, func in origin_gramars.items():
    left, right = func.split('->')
    right = split_symbol(right)
    tmp_gramar = Gramar(left, right, key)
    gramars_by_id[key] = tmp_gramar
    if left not in gramars_by_left.keys():
        gramars_by_left[left] = [tmp_gramar]
    else:
        gramars_by_left[left].append(tmp_gramar)


# 求first集和follow集

first: dict[str, set[str]] = {}
follow: dict[str, set[str]] = {}
for left in gramars_by_left.keys():
    first[left] = set()
    follow[left] = set()

# 求first集
while True:
    first_before = copy.deepcopy(first)
    for (key, exp) in gramars_by_left.items():
        for item in exp:
            item = item.right
            i = 0
            while i < len(item):
                c = item[i]
                if isterminator(c) or isempty(c):
                    first[key].add(c)
                    break
                else:
                    if (i == len(item) - 1):
                        first[key] = set.union(first[key], first[c])
                    else:
                        tmp = copy.deepcopy(first[c])
                        tmp.discard('ε')
                        first[key] = set.union(first[key], tmp)

                    if 'ε' not in first[c]:
                        break
                i += 1
    if first_before == first:
        break

print("first集:")

for key, item in first.items():
    print(f'{key}:\t', end='')
    for symbol in item:
        print(symbol + '\t', end='')
    print('')
print('')


# 求follow集
follow[gramars_by_id['0'].left].add('$')
while True:
    follow_before = copy.deepcopy(follow)
    for (key, exp) in gramars_by_left.items():
        for item in exp:
            item = item.right
            empty = True
            after_key = -1
            after_value = 0
            i = len(item) - 1
            while i >= 0:
                c = item[i]
                if isterminator(c) or isempty(c):
                    after_key = 0
                    after_value = c
                    empty = False
                else:
                    if after_key == 0:
                        follow[c].add(after_value)
                    elif after_key == 1:
                        follow[c] = set.union(follow[c], first[after_value])
                        follow[c].discard('ε')

                    if empty == True:
                        follow[c] = set.union(follow[c], follow[key])

                    after_key = 1
                    after_value = c
                    if 'ε' not in first[c]:
                        empty = False
                i -= 1

    if follow_before == follow:
        break

print("follow集:")
for key, item in follow.items():
    print(f'{key}:\t', end='')
    for symbol in item:
        print(symbol + '\t', end='')
    print('')
print('')


# 构建标准的LR(1)分析表


class Edge:
    def __init__(self, symbol, next_I):
        self.symbol: str = symbol
        self.next_I: I = next_I


class I_item:
    def __init__(self, gramar, point_pos, forward_search):
        self.gramar: Gramar = gramar
        self.point_pos: int = point_pos
        self.forward_search: set[str] = forward_search

    def is_over(self):
        return len(self.gramar.right) == self.point_pos

    def get_point_symbol(self):
        return self.gramar.right[self.point_pos]

    def get_left(self):
        if self.point_pos + 1 < len(self.gramar.right):
            return self.gramar.right[self.point_pos + 1]
        else:
            return ''


class I:
    tot = 0

    def __init__(self, items):
        self.edge: list[Edge] = []
        self.items: list[I_item] = items
        self.get_name()

    def get_name(self):
        # 利用tot获得名字
        self.name = 'I' + str(I.tot)
        I.tot += 1

    def add_item(self, new_item: I_item):
        for now_item in self.items:
            if new_item.gramar == now_item.gramar and new_item.point_pos == now_item.point_pos:
                if not(new_item.forward_search <= now_item.forward_search):
                    now_item.forward_search |= new_item.forward_search
                    return True
                else:
                    return False

        self.items.append(new_item)
        return True


def get_forward_search(next: str, forward_search: set[str]):
    if next == '':
        return forward_search
    result: set = set()
    for c in next:
        if isterminator(c):
            return result | set(c)
        elif isempty(c):
            continue
        else:
            result |= first[c]
            if 'ε' in first[c]:
                continue
            else:
                return result
    return result | forward_search


def cal_clusure(I_now: I):
    item_que = [item for item in I_now.items]
    while(len(item_que)):
        item_now = item_que.pop(0)
        if not item_now.is_over():
            if not isterminator(key := item_now.get_point_symbol()):
                new_forward_search = get_forward_search(
                    item_now.get_left(), item_now.forward_search)
                for gramar in gramars_by_left[key]:
                    new_i_item = I_item(
                        gramar, 0, copy.deepcopy(new_forward_search))
                    flag = I_now.add_item(new_i_item)
                    if flag:
                        if not (isterminator(symbol := new_i_item.get_point_symbol()) or isempty(symbol)):
                            item_que.append(new_i_item)


I0 = I([I_item(gramars_by_id['0'], 0, {'$'})])
cal_clusure(I0)
I_que: list[I] = [I0]
graph: list[I] = [I0]


def get_next_I(new_items: list[I_item]):
    for now_I in graph:
        is_exit_all_item = True
        for new_item in new_items:
            is_exit_single_item = False
            for old_item in now_I.items:
                if new_item.gramar == old_item.gramar and new_item.point_pos == old_item.point_pos and new_item.forward_search <= old_item.forward_search:
                    is_exit_single_item = True
            if not is_exit_single_item:
                is_exit_all_item = False
        if is_exit_all_item:
            return False, now_I
    return True, I(new_items)


while len(I_que):
    I_now = I_que.pop(0)
    # 移动到下一个I
    step_next_symbol_items_by_key: dict[str, list[I_item]] = {}
    for i_item in I_now.items:
        step_next_symbol_item = I_item(
            i_item.gramar, i_item.point_pos + 1, copy.deepcopy(i_item.forward_search))
        if not i_item.is_over():
            key = i_item.get_point_symbol()
            if key in step_next_symbol_items_by_key.keys():
                step_next_symbol_items_by_key[key].append(
                    step_next_symbol_item)
            else:
                step_next_symbol_items_by_key[key] = [
                    step_next_symbol_item]

    for key, next_symbol_item in step_next_symbol_items_by_key.items():
        flag, next_I = get_next_I(next_symbol_item)
        if flag:
            I_que.append(next_I)
            graph.append(next_I)
            cal_clusure(next_I)

        I_now.edge.append(Edge(key, next_I))

# 构建LR(1)项目族完成

# 构建LR(1)分析表
analy_table: dict[str, dict[str, str]] = {}

for I_now in graph:
    row = I_now.name
    analy_table[row] = {}
    for item in I_now.items:
        if item.is_over():
            for symbol in item.forward_search:
                val = 'R' + item.gramar.id
                if val == 'R0':
                    val = 'acc'
                if symbol in analy_table[row].keys():
                    raise RuntimeError("出现冲突，此文法不是LR(1)文法")
                else:
                    analy_table[row][symbol] = val

    for edge in I_now.edge:
        if isterminator(symbol := edge.symbol):
            val = 'S' + edge.next_I.name[1:]
        else:
            val = edge.next_I.name[1:]
        if symbol in analy_table[row].keys():
            raise RuntimeError("出现冲突，此文法不是LR(1)文法")
        else:
            analy_table[row][symbol] = val


print('标准的LR(1)分析表')
for row_name, row in analy_table.items():
    print(row_name + ':\t', end='')
    for key, item in row.items():
        print(key + ':' + item + '\t', end='')
    print('')
import json
json.dump(analy_table, open("/data/table.json"[1:], 'w'))
print('')

s = \
    """
支持浮点数和科学计数法，基本支持所有python支持的四则运算
有一定的错误处理能力，除0抛出自定义异常
样例  3++2  ,在python中是一个正确表达式
请输入表达式,例如 2.33e5*10/5+99-(-1) 
"""
expr = input(s)
print('')

tokenize(expr)
print('进行词法分析')
for item in lexical_result:
    print(item[0], end='')
print('\n')

# 初始化栈
stack = ['0']
val_stack = ['']


def action_zero():
    print(f'表达式值为{val_stack[-2]}')


def action_one():
    return val_stack[-6] + val_stack[-2]


def action_two():
    return val_stack[-6] - val_stack[-2]


def action_three():
    return val_stack[-2]


def action_four():
    return val_stack[-6] * val_stack[-2]


def action_five():
    if val_stack[-2] == 0:
        raise RuntimeError("除数不能为0")
    return val_stack[-6] / val_stack[-2]


def action_six():
    return val_stack[-2]


def action_seven():
    return val_stack[-4]


def action_eight():
    return val_stack[-2]


def need_eval(token: str):
    return token == 'id'


gramar_actions = {
    '0': action_zero,
    '1': action_one,
    '2': action_two,
    '3': action_three,
    '4': action_four,
    '5': action_five,
    '6': action_six,
    '7': action_seven,
    '8': action_eight,
}
print('开始进行LR(1)分析')
lexical_result.append(['$', ''])
while True:
    stack_status = 'I' + stack[-1]
    token, value = lexical_result[0]

    if token not in analy_table[stack_status]:
        lexical_result.pop(0)
        print(f'error:舍弃 {token} {value} ')
    else:
        action = analy_table[stack_status][token]
        if action == 'acc':
            print("接受")
            print("分析成功")
            print('')
            gramar_actions['0']()
            break
        else:
            op = action[0]
            label = action[1:]
            if op == 'S':
                stack.append(token)
                stack.append(label)
                if need_eval(token):
                    value = eval(value)
                val_stack.append(value)
                val_stack.append('')
                lexical_result.pop(0)
                print(f'移进 {token} {value}')
            elif op == 'R':
                tmp_gramar = gramars_by_id[label]
                left, right = tmp_gramar.left, tmp_gramar.right

                value = gramar_actions[label]()
                for symbol in reversed(right):
                    while symbol != stack[-1]:
                        stack.pop()
                        val_stack.pop()
                    stack.pop()
                    val_stack.pop()

                stack_status = 'I' + stack[-1]
                goto = analy_table[stack_status][left]

                stack.append(left)
                stack.append(goto)
                val_stack.append(value)
                val_stack.append('')

                print(f'按 {left}->{right} 规约')
