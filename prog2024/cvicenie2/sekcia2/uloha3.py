
def vypis(n: int) -> None:
    for i in range(2, n*2+1, 2):
        print(i)

if __name__ == "__main__":
    print("----")
    vypis(1)
    vypis(5)
    print("----")
    vypis(10)

