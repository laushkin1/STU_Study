
def nested_sum(zoznam_zoznamov_cisel: list[list]) -> int:
    sum_cisel = 0
    for i in zoznam_zoznamov_cisel:
        for j in i:
            sum_cisel += j

    return sum_cisel


if __name__ == "__main__":
    print(nested_sum([[5, -1], [0], [1, 10, 7, 9], [-4]]))

