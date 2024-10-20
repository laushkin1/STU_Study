
def mala_nasobilka() -> None:
    for i in range(11):
        for j in range(11):
            if i == 0 and j == 0: ...
            elif i == 0: i = 1
            elif j == 0: j = 1
            print(f"{i*j}\t", end="")
        print("\n", end="")

if __name__ == "__main__":
    mala_nasobilka()

