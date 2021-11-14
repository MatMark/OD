import math

import numpy as np
from tabulate import tabulate


def headings(size: int):
    x = np.array(np.ones((1, size), dtype=int), dtype=str)
    y = np.array(np.ones((1, size + 1), dtype=int), dtype=str)
    for i in range(2, size):
        x[0][i] = 'a'+str(i-1)
        y[0][i+1] = 'a'+str(i-1)
    x[0][0] = 0
    y[0][0] = '*'
    y[0][1] = 0
    return x, y


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


# 1 zadanie
print(tabulate((mulTable(2, 3)), tablefmt="fancy_grid"))
print(tabulate((mulTable(2, 4)), tablefmt="fancy_grid"))
