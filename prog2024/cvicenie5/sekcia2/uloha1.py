
def je_mocnina(a: int, b: int) -> bool:
    if a == 1: return True
    if a%b: return False
    elif a == b: return True
    return je_mocnina(a/b, b)

if __name__ == "__main__":
    print(je_mocnina(8, 2))
    print(je_mocnina(1, 3))
    print(je_mocnina(81, 3))
    print(je_mocnina(80, 3))

