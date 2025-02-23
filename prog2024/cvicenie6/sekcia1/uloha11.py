
def modulo(a: int, b: int) -> int|None:
    if b == 0: return None

    while a != 0:
        if a >= b:
            a = a - b
        else: break
    return a



if __name__ == "__main__":
    print(modulo(6, 3))
    print(modulo(1, 4))
    print(modulo(3, 0))
    print(modulo(11, 3))

