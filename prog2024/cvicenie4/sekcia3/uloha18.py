
def pohyb_jazdca(x1: int, y1: int, x2: int, y2: int) -> bool:
    if not all(i < 9 and i > 0 for i in [x1, x2, y1, y2]):
        return False

    if (y2 == y1+2 or y2 == y1-2) and (x2==x1-1 or x2==x1+1):
        return True
    elif (x2 == x1+2 or x2 == x1-2) and (y2==y1-1 or y2==y1+1):
        return True
    return False

if __name__ == "__main__":
    print(pohyb_jazdca(5, 4, 3, 3))
    print(pohyb_jazdca(5, 4, 6, 5))
    print(pohyb_jazdca(2, 4, 3, 2))
    print(pohyb_jazdca(9, 4, 3, 3))

