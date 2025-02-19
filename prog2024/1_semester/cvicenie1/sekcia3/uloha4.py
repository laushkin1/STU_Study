import turtle
from time import sleep
SEC = 1
LINE = 120

def ul4(home, LINE):
    """_summary_

    Args:
        home (turtle.Vec2D): turtle.position()
        LINE (int): 60
    """
    turtle.forward(LINE)
    p = turtle.position()

    for i in range(6):
        turtle.goto(p)
        turtle.left(90)
        turtle.forward(LINE)
        p = turtle.position()
        if i == 5: break # !!!
        turtle.goto(home)
        turtle.setheading(turtle.towards(p))

if __name__ == "__main__":
    turtle.teleport(-120, 0)
    ul4(turtle.position(), LINE=LINE)
    sleep(SEC*10)

