import turtle
from time import sleep

from uloha2 import n_uholka, SEC, LINE


def make_fractal(n):
    for _ in range(n):
        turtle.left((180/n)*2)
        n_uholka(int(n/2))

if __name__ == "__main__":

    for i in range(6, 100, 4):
        make_fractal(i)
        sleep(SEC*2)
        turtle.clear()

    # sleep(SEC*10)
