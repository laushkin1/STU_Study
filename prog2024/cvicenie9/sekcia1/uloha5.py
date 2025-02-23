
def pocet_lokalnych_maxim(zoznam_cisel: list[int]) -> int:
    if len(zoznam_cisel) <= 2: return 0

    if zoznam_cisel[0] < zoznam_cisel[1] > zoznam_cisel[2]:
        return pocet_lokalnych_maxim(zoznam_cisel[1:]) + 1

    return pocet_lokalnych_maxim(zoznam_cisel[1:])


if __name__ == "__main__":
    print(pocet_lokalnych_maxim([2, 5, 3, -2, -3, 0, -1]))

