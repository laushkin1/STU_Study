
def je_vzostupne(slovo: str) -> bool:
    for i in range(len(slovo)-1):
        i = i+1
        if ord(slovo[i-1]) > ord(slovo[i]):
            return False
    return True


if __name__ == "__main__":
    count = 0
    words = open("../words.txt")
    for word in words:
        word = word.strip()
        if je_vzostupne(word):
            print(word)
            count += 1
    print("\n")
    print(f"{count} words")

    print("\n")
    print(je_vzostupne("stlp"))
    print(je_vzostupne("psy"))
    print(je_vzostupne("abeceda"))
    print(je_vzostupne("mor"))

