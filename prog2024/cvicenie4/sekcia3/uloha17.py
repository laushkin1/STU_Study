from uloha13 import pohyb_veze
from uloha16 import pohyb_strelca

def pohyb_damy(x1: int, y1: int, x2: int, y2: int) -> bool:
    if pohyb_veze(x1, y1, x2, y2) or pohyb_strelca(x1, y1, x2, y2):
        return True
    return False


if __name__ == "__main__":
    print(pohyb_damy(3, 3, 8, 3))
    print(pohyb_damy(3, 3, 9, 3))
    print(pohyb_damy(3, 3, 1, 5))
    print(pohyb_damy(3, 3, 5, 4))

