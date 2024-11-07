
def pocet_vyskytov(retazec: str, podretazec: str) -> int:
    return retazec.count(podretazec)

if __name__ == "__main__":
    print(pocet_vyskytov("banana", "a"))
    print(pocet_vyskytov("banana", "an"))
    print(pocet_vyskytov("banana", "ana"))

