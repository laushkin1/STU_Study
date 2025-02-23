
def pocet_rovnych(zoznam: list) -> int:
    tem_list = []
    [tem_list.append(i) for i in zoznam if i not in tem_list]

    return len(tem_list)

if __name__ == "__main__":
    print(pocet_rovnych([2, "a", [1,2], "a", [1,2], 3, "a"]))

