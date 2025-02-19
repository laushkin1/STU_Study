from random import randint
from player import move_player


def create_field(n: int) -> list:
    # create empty map
    field = []

    for i in range(n):
        field.append([i for i in "."*n])

    return field


def print_map(our_map: [list], players: [list] = None) -> None:
    len_map = len(our_map)

    # make copy of map with players (if players exist)
    copy_map = []
    for i in range(len_map):
        copy_map.append([our_map[i][j] for j in range(len_map)])
    if players != None:
        for i in range(len(players)):
            copy_map[players[i][0]][players[i][1]] = str(i+1)

    print("The map: ")

    # print indexs of rows
    print(" ", end="")
    for i in range(len_map):
        print(f"\033[36m {i}\033[0m", end="")
    print(" ")

    # print new map
    for indx in range(len_map):

        # print indexs of cols
        print(f"\033[36m{indx}\033[0m", end="")

        # print elements
        for i in range(len_map):
            # paint elements
            if copy_map[indx][i] in "ABCDE":
                print(f"\033[32m {copy_map[indx][i]}\033[0m", end="")
            elif copy_map[indx][i] in "abcde":
                print(f"\033[31m {copy_map[indx][i]}\033[0m", end="")
            elif copy_map[indx][i] in "+*":
                print(f"\033[33m {copy_map[indx][i]}\033[0m", end="")
            else: print(f" {copy_map[indx][i]}", end="")
        print(" ")

    print("="*(len_map*2+1))



def add_start_end_position(our_map: [list]) -> None:
    len_map = len(our_map)
    our_map[0][0] = '+'
    if not len_map % 2:
        our_map[len_map-1][0] = '*'
    else: our_map[len_map-1][len_map-1] = '*'


def generate_coordinates_for_teleport(our_map: [list], previus: list = [0, 0]) -> list:
    last_indx = len(our_map) - 1

    # if it's the beginning of a teleport, it doesn't matter where it is
    if previus == [0, 0]:
        while True:
            random_x = randint(0, last_indx)
            random_y = randint(0, last_indx)

            if our_map[random_x][random_y] == '.':
                break

    # if it's not the beginning of a teleport - teleports shouldn't be on the same line
    while True:
        random_x = randint(0, last_indx)
        random_y = randint(0, last_indx)

        if our_map[random_x][random_y] == '.' and random_x != previus[0]:
            break

    return [random_x, random_y]


def add_teleports(our_map: [list]) -> None:
    len_map = len(our_map)

    # add positive teleports
    for i in range(len_map//2):
        p_tlp = generate_coordinates_for_teleport(our_map)
        our_map[p_tlp[0]][p_tlp[1]] = chr(65+i)

        p_tlp_e = generate_coordinates_for_teleport(our_map, p_tlp)
        our_map[p_tlp_e[0]][p_tlp_e[1]] = chr(65+i)


    # add negative teleports
    for i in range(len_map//2):
        n_tlp = generate_coordinates_for_teleport(our_map)
        our_map[n_tlp[0]][n_tlp[1]] = chr(97+i)

        n_tlp_e = generate_coordinates_for_teleport(our_map, n_tlp)
        our_map[n_tlp_e[0]][n_tlp_e[1]] = chr(97+i)


def create_map(map_size: int) -> [list]:
    our_map = create_field(map_size)
    add_start_end_position(our_map)
    add_teleports(our_map)

    return our_map

