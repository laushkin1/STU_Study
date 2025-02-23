
def pohyb_krala(x1: int, y1: int, x2: int, y2: int) -> bool:
    if not all(i < 9 and i > 0 for i in [x1, x2, y1, y2]):
       return False

    if (x1+1==x2 or x1-1==x2) and (y1+1==y2 or y1-1==y2 or y1==y2): return True
    elif y1+1==y2 or y1-1==y2: return True
    return False

if __name__ == "__main__":
    print(pohyb_krala(4, 4, 5, 5))
    print(pohyb_krala(3, 3, 4, 3))
    print(pohyb_krala(3, 3, 4, 5))
    print(pohyb_krala(1, 1, 0, 1))

