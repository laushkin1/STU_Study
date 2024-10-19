import turtle
from time import sleep
SEC = 1
LINE = 15


for i in range(5):
    turtle.forward(LINE)
    turtle.left(90)
    turtle.forward(LINE)
    turtle.right(90)

turtle.forward(LINE)

for i in range(5):
    turtle.right(90)
    turtle.forward(LINE)
    turtle.left(90)
    turtle.forward(LINE)

turtle.right(90)
turtle.forward(LINE)

for i in range(5):
    turtle.right(90)
    turtle.forward(LINE)
    turtle.left(90)
    turtle.forward(LINE)

turtle.right(90)
turtle.forward(LINE)
turtle.right(90)

for i in range(5):
    turtle.forward(LINE)
    turtle.left(90)
    turtle.forward(LINE)
    turtle.right(90)

turtle.forward(LINE)

sleep(SEC*5)

