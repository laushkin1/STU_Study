
def sucet_stvorcov(n: int) -> int:
    res = 0
    for i in range(1, n+1):
        res += i*i
    return res

if __name__ == "__main__":
    print(sucet_stvorcov(3))
    print(sucet_stvorcov(5))

