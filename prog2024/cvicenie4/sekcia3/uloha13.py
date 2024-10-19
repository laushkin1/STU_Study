

def pohyb_veze(x1: int, x2: int, x3: int, x4: int) -> bool:
    if not all(i < 9 and i > 0 for i in [x1, x2, x3, x4]):
        return False
    if x1 == x3 or x2 == x4: return True
    return False


if __name__ == "__main__":
    print(pohyb_veze(3, 3, 3, 7))
    print(pohyb_veze(3, 3, 4, 7))
    print(pohyb_veze(3, 3, 7, 3))
    print(pohyb_veze(3, 3, 3, 9))
