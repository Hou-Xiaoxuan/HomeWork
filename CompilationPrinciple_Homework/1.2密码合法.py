'''
Author: LinXuan
Date: 2021-09-15 14:35:50
Description: 附加题，检测密码是否符合要求
LastEditors: LinXuan
LastEditTime: 2021-09-15 15:44:41
FilePath: /c++/CompilationPrinciple/1.2密码合法.py
'''
import re
def check_type(password):
    """使用正则检查输入的密码是否符合规范, 使用排除法，排除不符合规范的密码"""
    invalid_pattern =r'^([a-zA-Z0-9]+|[a-zA-Z\W]+|[a-z0-9\W]+|[A-Z0-9\W]+|.{0,7})$'
    invalid_re = re.compile(invalid_pattern)
    if invalid_re.match(password) == None:
        return True
    else:
        return False

def check_history(password, history):
    """从历史密码中检查是否重复了三个字符以上"""
    repeat = set()
    for ch in password:
        if ch in history:
            repeat.add(ch)
    return len(repeat) < 3

def add_history(password, history):
    for ch in password:
        history.add(ch)

def main():
    history = set()
    while(True):
        print("请设置密码: ", end="")
        password = input()
        if check_type(password) == False:
            print("密码格式不符合要求")
        elif check_history(password, history) == False:
            print("密码重复字符超过3个")
        else:
            print("密码合法！已经计入历史")
            add_history(password, history)
            print("---------------------")
            print("时间已过，密码失效。")
        

if __name__ == "__main__":
    main()
