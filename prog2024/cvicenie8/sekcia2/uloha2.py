
def neobsahuje_e(slovo: str) -> bool:
    for c in slovo:
        if c == 'e':
            return False

    return True

if __name__ == "__main__":
    print(neobsahuje_e("asdaawf"))
    print(neobsahuje_e("poipokl"))
    print(neobsahuje_e("wersdfwe"))

