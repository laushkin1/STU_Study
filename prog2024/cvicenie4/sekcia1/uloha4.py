import turtle
from time import sleep

def menu() -> None:
    while True:
        user_input = input("Please enter letter: ").strip()
        if user_input == "s":
            for _ in range(4):
                turtle.forward(100)
                turtle.right(90)
        elif user_input == "t":
            for _ in range(3):
                turtle.forward(100)
                turtle.left(120)
        elif user_input == "e":
            break
        else:
            print("Input is incorrect")

        sleep(2)
        turtle.clear()


if __name__ == "__main__":
    menu()

