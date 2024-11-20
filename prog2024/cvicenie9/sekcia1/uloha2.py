
def cumsum(zoznam_cisel: list[int]) -> list[int]:
    new_list = []
    a = 0
    for i in zoznam_cisel:
        new_list.append(i + a)
        a += i

    return new_list

if __name__ == "__main__":
    print(cumsum([5, 1, -2, 3]))

