
def mince(suma: int, pocet: int, coins=[0.50, 0.20, 0.10, 0.05, 0.02, 0.01]) -> bool:
    if suma == 0 and pocet == 0: return True
    if suma < 0 or pocet == 0: return False


    for coin in coins:
        if mince(round(suma - coin, 2), pocet - 1):
            return True

    return False

if __name__ == "__main__":
    print(mince(suma=0.04, pocet=4))
    print(mince(suma=0.60, pocet=2))
    print(mince(suma=0.31, pocet=2))
    print(mince(suma=0.31, pocet=3))

