
def pouziva_vsetky(slovo: str, pismena: str) -> bool:
    for s in slovo:
        for p in pismena:
            if s == p:
                pismena = pismena.replace(p, '')

    if pismena: return False
    return True

if __name__ == "__main__":
    print(pouziva_vsetky("stlp", "prstl"))
    print(pouziva_vsetky("stlpy", "pstl"))
    print(pouziva_vsetky("abeceda", "abcde"))
    print(pouziva_vsetky("abeciedka", "abcde"))
    print(pouziva_vsetky("abeciedka", "abcdef"))

