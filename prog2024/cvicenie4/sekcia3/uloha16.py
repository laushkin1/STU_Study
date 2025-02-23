
def pohyb_strelca(x1: int, y1: int, x2: int, y2: int) -> bool:
    if not all(i < 9 and i > 0 for i in [x1, x2, y1, y2]):
        return False

    if abs(x1-x2) == abs(y1-y2): return True
    return False


if __name__ == "__main__":
    print(pohyb_strelca(3, 3, 1, 5))
    print(pohyb_strelca(3, 3, 1, 4))

