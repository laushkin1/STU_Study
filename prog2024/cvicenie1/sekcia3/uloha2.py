import turtle
from time import sleep

SEC = 1
LINE = 60

def n_uholka(n):
    d = (180/n)*2
    for _ in range(n):
        turtle.forward(LINE)
        turtle.left(d)

if __name__ == "__main__":
    print("Hello, enter 2 or more for for your n-corner")
    while(True):
        n = int(input("Please enter n: "))
        turtle.clear()
        if n < 3:
            print("Goodbye!~")
            break
        n_uholka(n)

