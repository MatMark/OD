import numpy as np
from tabulate import tabulate


def user_input():
    while(True):
        val = input("Podaj p dla ciała GF(p): ")
        try:
            p = int(val)
            if(p <= 19):
                break
            else:
                print("p musi być liczbą całkowitą ≤ 19")
        except ValueError:
            print("p musi być liczbą całkowitą ≤ 19")
    return p


def add_table(p):
    # appearance only
    table = np.zeros(shape=[p, p + 1])
    headers = ['+']
    for i in range(0, p + 1):
        headers = np.append(headers, i)
        if(i < p):
            table[i, 0] = i

    # main functionality
    for i in range(0, p + 1):
        if(i < p):
            for j in range(0, p + 1):
                if(j < p):
                    table[j, i+1] = (i + j) % p

    print("Tabela dodawania:")
    print(tabulate(table, headers, tablefmt="fancy_grid"))


def mul_table(p):
    # appearance only
    table = np.zeros(shape=[p, p + 1])
    headers = ['*']
    for i in range(0, p + 1):
        headers = np.append(headers, i)
        if(i < p):
            table[i, 0] = i

    # main functionality
    for i in range(0, p + 1):
        if(i < p):
            for j in range(0, p + 1):
                if(j < p):
                    table[j, i+1] = (i * j) % p

    print("Tabela mnożenia:")
    print(tabulate(table, headers, tablefmt="fancy_grid"))


def opposite(p):
    table = np.zeros(shape=[2, p])
    for i in range(0, p):
        table[0, i] = i
        table[1, i] = -i % p
    print("Przeciwne:")
    print(tabulate(table, tablefmt="fancy_grid"))


# def invert(p):
#     table = np.zeros(shape=[2, p-1])
#     for i in range(1, p):
#         table[0, i-1] = i
#         table[1, i-1] = (i*i)%p
#     print("Odwrotne:")
#     print(tabulate(table, tablefmt="fancy_grid"))


def multiplicative(p):
    table = np.zeros(shape=[2, p])
    for i in range(1, p):
        table[0, i] = i
        if(i < p):
            for j in range(1, p + 1):
                if(pow(i, j) % p == 1):
                    break
            if(j != p):
                table[1, i] = j
            else:
                table[1, i] = -1
    print("Rząd multiplikatywny:")
    # table[:,1:] pominięcie pierwszej kolumny
    print(tabulate(table[:, 1:], tablefmt="fancy_grid"))


p = user_input()
add_table(p)
mul_table(p)
opposite(p)
# invert(p)
multiplicative(p)
