from uloha10 import vynasob

def umocnik(n: int, k: int) -> int:
    r = n
    for _ in range(k-1):
        r = vynasob(r, n)
    return r


if __name__ == "__main__":
    print(umocnik(1, 5))
    print(umocnik(2, 5))
    print(umocnik(3, 4))

