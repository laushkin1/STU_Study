
def um(n: int, x: int) -> int:
    r = 1
    for i in range(x):
        r *= n

    return r


def fejkovy_logaritmus(n: int) -> int:
    x = 0
    while True:
        x+=1
        if um(2, x) >= n:
            break
    return x-1


if __name__ == "__main__":
    print(fejkovy_logaritmus(10))
    print(fejkovy_logaritmus(33))
    print(fejkovy_logaritmus(32))

