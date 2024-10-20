
def grid(n: int) -> None:
    if n == 1:
        print("N must be <= 2")
    else:

        print("+", end="")
        print(" - - - - +"*n, end="")
        print("\n", end="")

        for i in range(n):
            for i in range(4):
                print("|", end="")
                print("         |"*n, end="")
                print("\n", end="")

            print("+", end="")
            print(" - - - - +"*n, end="")
            print("\n", end="")


if __name__ == "__main__":
    grid(3)

