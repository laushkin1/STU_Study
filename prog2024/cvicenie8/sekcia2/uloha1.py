
def slova_dlhsie_nez(n: int):
    file = open("../words.txt", "r")
    for line in file:
        word = line.strip()
        if len(word) >= n:
            print(word)

    file.close()

if __name__ == "__main__":
    slova_dlhsie_nez(21)

