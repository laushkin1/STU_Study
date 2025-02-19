from uloha10 import vynasob


def tretia_mocnina(a: int) -> int:
    r = vynasob(a, a)
    return vynasob(r, a)

if __name__ == "__main__":
    print(tretia_mocnina(3))
    print(tretia_mocnina(4))

