
def cezar(OT: str, posun: int) -> str:
    new_OT = ""

    while posun >= 26:
        posun -= 26

    for c in OT:
        new_OT += chr(ord(c) + posun)

    return new_OT

if __name__ == "__main__":
    print(cezar("CHEER", 7))
    print(cezar("AHOJ", 1))
    print(cezar("AHOJ", 26))

