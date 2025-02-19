
def stred(t: list) -> list|None:
    if t == []: return None
    return t[1:-1]


if __name__ == "__main__":
    z = [5, [], 'a', [1,2,3], -5.5]
    print(z)
    t = stred(z)
    print('   ', end="")
    print(t)
    print(z)

    print(stred([]))

