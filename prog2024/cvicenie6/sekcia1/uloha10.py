
def kvocient(a: int, b: int) -> int|None:
    if b == 0: return None

    count = 0
    while a != 0:
        if a >= b:
            a = a - b
            count += 1
        else: break
    return count

if __name__ == "__main__":
    print(kvocient(6, 3))
    print(kvocient(1, 4))
    print(kvocient(3, 0))
    print(kvocient(11, 3))

