from math import modf

def stvorce_mensie_n(n: int) -> None:
    m = 0
    while True:
        m += 1
        if n == m:
            break

        sn, fn = modf(m**0.5)
        if sn == 0 and fn != 0:
            print(f"{m} ", end="")

    print()


if __name__ == "__main__":
    stvorce_mensie_n(10)
    stvorce_mensie_n(25)
    stvorce_mensie_n(26)

