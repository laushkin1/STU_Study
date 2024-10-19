from uloha11 import delitelnost

def je_prvocislo(a: int) -> bool:
    for i in range(2, a):
        if delitelnost(a, i): return False

    return True


if __name__ == "__main__":
    print(je_prvocislo(97))
    print(je_prvocislo(87))
    print(je_prvocislo(71))

