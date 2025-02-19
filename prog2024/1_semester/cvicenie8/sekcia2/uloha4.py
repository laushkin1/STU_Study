
def pouziva_len(slovo: str, pismena: str) -> bool:
    for s in slovo:
        pouziva = False
        for p in pismena:
            if s == p:
                pouziva = True

        if pouziva == False:
            return False

    return True

if __name__ == "__main__":
    print(pouziva_len("stlp", "prstl"))
    print(pouziva_len("stlpy", "prstl"))
    print(pouziva_len("abeceda", "abcdefgh"))
    print(pouziva_len("abeciedka", "abcdefgh"))

