
def je_prvocislo(a: int) -> bool:
    if a == 1: return False
    for i in range(2, a):
        if not a%i: return False

    return True

def sucet_prvocisel(n: int) -> int:
    if not hasattr(sucet_prvocisel, "first_call"):
        sucet_prvocisel.first_call = True

    if sucet_prvocisel.first_call:
        sucet_prvocisel.first_call = False
        return sucet_prvocisel(n-1)

    if n <= 1: return 0

    if je_prvocislo(n):
        return n + sucet_prvocisel(n-1)
    else:
        return sucet_prvocisel(n-1)

if __name__ == "__main__":
    print(sucet_prvocisel(10))
    print(sucet_prvocisel(14))
    print(sucet_prvocisel(13))

