import math

import numpy as np
from tabulate import tabulate
from helpers import headings, user_input, menu


ZechTable = {
    4: (2, 1),
    8: (3, 6, 1, 5, 4, 2),
    16: (4, 8, 14, 1, 10, 13, 9, 2, 7, 5, 12, 11, 6, 3)
}


def inverse(x: int, p: int, q: int):
    if (x != 0 and p > 2):
        return int((x + (q - 1) / 2) % (q - 1))
    elif (x != 0 and p == 2):
        return x
    elif (x == 0):
        return 0


def mulTable(p: int, m: int):
    size = int(math.pow(p, m))
    table = np.array(np.zeros((size, size), dtype=int), dtype=str)

    for i in range(1, size):
        for j in range(1, size):
            if((i - 1 + j - 1) % (size - 1) == 0):
                table[i][j] = 1
            else:
                table[i][j] = 'a'+str((i - 1 + j - 1) % (size - 1))

    # adding headings
    x, y = headings(size)
    table = np.insert(table, 0, x, axis=0)
    table = np.insert(table, 0, y, axis=1)
    return table


def add(x: int, y: int, p: int, m: int):
    q = int(math.pow(p, m))
    if (y > x):
        x, y = y, x
    if (x != 0 and y != 0 and x > y):
        zech = ZechTable.get(q)[x - y - 1]
        return ((y + zech - 1) % (q - 1) + 1)
    elif (y == 0 or x == 0):
        return x + y
    elif (x != 0 and y == inverse(x, p, q)):
        return 0


def mul(x: int, y: int, p: int, m: int):
    q = int(math.pow(p, m))
    if (x > 0 and y > 0):
        return 1 + (x + y - 2) % (q - 1)
    else:
        return 0


def zad1():
    print(tabulate((mulTable(2, 3)), tablefmt="fancy_grid"))
    print(tabulate((mulTable(2, 4)), tablefmt="fancy_grid"))


def zad2():
    x = user_input('Pierwsza liczba: ')
    y = user_input('Druga liczba: ')
    p = user_input('p: ')
    m = user_input('m: ')
    print('{} + {} ='.format(x, y), add(x, y, p, m))
    print('{} * {} ='.format(x, y), mul(x, y, p, m))


menu({1: zad1, 2: zad2})
