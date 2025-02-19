
def je_zoradeny(zoznam_cisel: list[int]) -> bool:
    return all(zoznam_cisel[i] <= zoznam_cisel[i+1] for i in range(len(zoznam_cisel) - 1))


if __name__ == "__main__":
    print(je_zoradeny([1,2,2,3]))
    print(je_zoradeny([3,1,2,3]))

