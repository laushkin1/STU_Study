
def je_prvocislo(a: int) -> bool:
    if a == 1: return False
    for i in range(2, a):
        if not a%i: return False

    return True

def faktorizacia(n: int) -> None:
    if je_prvocislo(n):
        print(n)
        return None
    for i in range(2, n):
        if not n%i:
            print(i)
            return faktorizacia(int(n/i))

if __name__ == "__main__":
    faktorizacia(80850)

