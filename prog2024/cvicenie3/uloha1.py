import turtle
from time import sleep
from math import sin, radians

turtle.speed("fastest") # change to "normal" to make speed of turtle normal
LINE = 50 # Change to make polynom bigger


def utvar(n: int, strana: int) -> None:
    ex_ang = 360/n # exterior angle / also central angle
    trg_ang = (180 - ex_ang) / 2  # triangle angle
    radius = (strana / (2 * (sin(radians(180 / n)))))  # Radius of a regular polynom

    # Make figure
    for _ in range(n):
        turtle.forward(strana)
        turtle.left(ex_ang + trg_ang)
        turtle.forward(radius)
        turtle.left(180-ex_ang)
        turtle.forward(radius)
        turtle.left(ex_ang + trg_ang)
        turtle.forward(strana)
        turtle.left(ex_ang)



if __name__ == '__main__':
    for i in range(3, 100):
        utvar(i, LINE)
        sleep(1) # change to see polynom longer
        turtle.clear()

