from re import search

def tri_dvojite(ret: str) -> bool:
    pattern = r'(?i)([a-z])\1([a-z])\2([a-z])\3'
    return bool(search(pattern, ret))

if __name__ == "__main__":
    count = 0
    words = open("../words.txt")
    for word in words:
        word = word.strip()
        if tri_dvojite(word):
            print(word)
            count += 1
    print("\n")
    print(f"{count} words")

    print("\n")
    print(tri_dvojite("committee"))
    print(tri_dvojite("commttee"))

