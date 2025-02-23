from collections import Counter

def pocet_rovnakych(a: int, b: int, c: int) -> int:
    r = Counter([a, b, c]).most_common(1)[0][1]
    return r if r>1 else 0

if __name__ == "__main__":
    print(pocet_rovnakych(6, 3, 4))
    print(pocet_rovnakych(6, 3, 6))
    print(pocet_rovnakych(6, 6, 6))

