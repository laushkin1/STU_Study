import turtle
from time import sleep
from math import sin, radians

turtle.speed("normal") # ccan be hanged to "fastest"
LINE = 50 # Change to make polynom bigger


def obluk(n: int, strana: int) -> None:
    ex_ang = 360/n # exterior angle / also central angle
    trg_ang = (180 - ex_ang) / 2  # triangle angle
    radius = (strana / (2 * (sin(radians(180 / n)))))  # Radius of a regular polynom

    # Make figure
    for _ in range(n):
        turtle.forward(strana)
        turtle.left(ex_ang + trg_ang)
        a = turtle.position()
        turtle.forward(radius)
        turtle.left(180 - ex_ang)
        b = turtle.position()
        turtle.forward(radius)
        turtle.left(ex_ang + trg_ang)
        c = turtle.position()
        turtle.forward(strana)
        turtle.left(ex_ang)



if __name__ == '__main__':
    for i in range(5, 100):
        obluk(i, LINE)
        sleep(2) # change to see polynom longer
        turtle.clear()

