
def je_palindrom(ret: str) -> bool:
    if len(ret) <= 1: return True
    if ret[0] == ret[-1]: return je_palindrom(ret[1:-1])
    return False

if __name__ == "__main__":
    print(je_palindrom("anna"))
    print(je_palindrom("anka"))
    print(je_palindrom("kobylamamalybok"))
    print(je_palindrom(""))

