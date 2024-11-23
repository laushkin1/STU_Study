
def pocet_unikatnych(zoznam: list) -> int:
    count = 0
    for i in zoznam:
        if zoznam.count(i) > 1:
            count += 1

    return len(zoznam) - count

if __name__ == "__main__":
    print(pocet_unikatnych([2, "a", [1,2], "a", [1,2], 3]))

