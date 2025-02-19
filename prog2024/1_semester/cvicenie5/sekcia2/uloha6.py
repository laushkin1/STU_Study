
def hanojske_veze(n: int, start: str, pomocny: str, koniec: str) -> None:
    if n == 1: print(f"{n}: {start} -> {koniec}")
    else:
        hanojske_veze(n-1, start, koniec, pomocny)
        print(f"{n}: {start} -> {koniec}")
        hanojske_veze(n-1, pomocny, start, koniec)


def hanojske_veze_pocet_presunov(n: int) -> int:
    return (2**n) - 1

if __name__ == "__main__":
    hanojske_veze(4, 'A', 'B', 'C')
    print(hanojske_veze_pocet_presunov(4))

