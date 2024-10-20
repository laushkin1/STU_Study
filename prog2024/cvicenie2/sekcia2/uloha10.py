
def vynasob(a: int, b: int) -> int:
    r = 0
    for _ in range(b):
        r+=a
    return r


if __name__ == "__main__":
    print(vynasob(2, 5))
    print(vynasob(5, 2))
    print(vynasob(5, 5))

