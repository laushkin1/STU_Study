
def sucet_parnych_pozicii(zoznam_cisel: list[int]) -> int:
    return sum(zoznam_cisel[i] for i in range(len(zoznam_cisel)) if i%2==1)


if __name__ == "__main__":
    print(sucet_parnych_pozicii([2, 5, 3, -2, -3, 0, 1]))

