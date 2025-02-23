
def neobsahuje(slovo: str, pismena: str) -> bool:
    for p in pismena:
        for s in slovo:
            if p == s:
                return False

    return True

if __name__ == "__main__":
    print(neobsahuje("stlp", "aeiouy"))
    print(neobsahuje("stlpy", "aeiouy"))
    print(neobsahuje("abeceda", "fg"))
    print(neobsahuje("abeceda", "efg"))

