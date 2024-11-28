
def stred(t: list) -> list|None:
    if t == []: return None

    t.pop(0)
    t.pop(-1)
    return t


if __name__ == "__main__":
    z = [5, [], 'a', [1,2,3], -5.5]
    print(z)
    stred(z)
    print(z)

    print(stred([]))

