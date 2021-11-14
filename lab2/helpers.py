import numpy as np


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


def headers_table(size: int, operator: str):
    x = np.array(np.ones((1, size), dtype=int), dtype=str)
    y = np.array(np.ones((1, size + 1), dtype=int), dtype=str)
    for i in range(2, size):
        x[0][i] = i
        y[0][i+1] = i
    x[0][0] = 0
    y[0][0] = operator
    y[0][1] = 0
    return x, y


def menu(options: dict):
    print('\n'.join('{}: {}'.format(k, v.__name__)
                    for k, v in options.items()))
    val = user_input_max('Wybierz zadanie: ', max=len(options)+1)
    if(val == 0):
        return
    else:
        options[val]()


def user_input_max(text: str, max: int = None):
    while(True):
        val = input(text)
        try:
            val = int(val)
            if(val <= max):
                break
            else:
                print("Wartosc musi być liczbą całkowitą ≤ {}".format(max))
        except ValueError:
            print("Wartosc musi być liczbą całkowitą ≤ {}".format(max))
    return val


def user_input(text: str):
    while(True):
        val = input(text)
        try:
            val = int(val)
            break
        except ValueError:
            print("Wartosc musi być liczbą całkowitą")
    return val
