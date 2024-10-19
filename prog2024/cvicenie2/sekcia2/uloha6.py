
def aritmeticka_postupnost(a0:int, d:int, N:int) -> None:
    print(a0)
    for _ in range(1, N):
        a0 = a0 + d
        print(a0)

if __name__ == "__main__":
    aritmeticka_postupnost(1,2,5)
    # aritmeticka_postupnost(5,-3,4)

