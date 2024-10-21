
def mince(suma: int, pocet: int, coins=[0.50, 0.20, 0.10, 0.05, 0.02, 0.01], used_coins=None) -> (bool, list):
    if used_coins is None:
        used_coins = []

    if suma == 0 and pocet == 0: return True, used_coins
    if suma < 0 or pocet == 0: return False, None

    for coin in coins:
        success, result = mince(round(suma - coin, 2), pocet - 1, coins, used_coins + [coin])

        if success:
            return True, result

    return False, None

if __name__ == "__main__":
    b, c = mince(suma=0.04, pocet=4)
    print(f"{b}: {c}")
    b, c = mince(suma=0.60, pocet=2)
    print(f"{b}: {c}")
    b, c = mince(suma=0.31, pocet=2)
    print(f"{b}: {c}")
    b, c = mince(suma=0.31, pocet=3)
    print(f"{b}: {c}")

