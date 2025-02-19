
def sucet_rekurzivne(n: int) -> int:
    if n<= 1: return 1
    return n + sucet_rekurzivne(n-1)

if __name__ == "__main__":
    print(sucet_rekurzivne(1))
    print(sucet_rekurzivne(2))
    print(sucet_rekurzivne(3))
    print(sucet_rekurzivne(10))

