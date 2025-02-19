
def GCD(a: int, b: int) -> int:
    if a == 0: return b
    if b == 0: return a
    return GCD(b, a%b)

if __name__ == "__main__":
    print(GCD(10, 15))
    print(GCD(30,  40))
    print(GCD(20, 27))

