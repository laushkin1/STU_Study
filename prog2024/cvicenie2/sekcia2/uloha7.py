
def geometricka_postupnost(a0:int, q:int, N:int) -> None:
    print(a0)
    for _ in range(1, N):
        a0 = a0 * q
        print(a0)

if __name__ == "__main__":
    geometricka_postupnost(1,2,5)
    # geometricka_postupnost(2,3,4)

