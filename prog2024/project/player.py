
def can_move_teleport(player: list, our_map: [list]) -> int:
    len_map = len(our_map)

    # Check and teleport positive teleport
    if our_map[player[0]][player[1]] in "ABCDE":
        for i in range(player[0]+1, len_map):
            for j in range(0, len_map):
                if our_map[i][j] == our_map[player[0]][player[1]]:
                    player[0] = i
                    player[1] = j
                    return 1


    # Check and teleport negative teleport
    if our_map[player[0]][player[1]] in "abcde":
        for i in range(player[0]):
            for j in range(0, len_map):
                if our_map[i][j] == our_map[player[0]][player[1]]:
                    player[0] = i
                    player[1] = j
                    return -1

    return 0


def move_player(player: list, moves: int, our_map: [list]) -> bool:
    # func changes player list and return True if list has been changed
    # and false if move is not allow

    len_map = len(our_map)
    x = player[0]
    y = player[1]

    if x % 2: direction = -1
    else: direction = 1

    counter = 0
    for _ in range(moves):
        counter += 1
        y += direction

        if y >= len_map:
            y = len_map - 1
            x += 1
            direction = -1
        elif y < 0:
            y = 0
            x += 1
            direction = 1

        if x >= len_map:
            x = len_map - 1
            counter -= 1
            break

    if counter == moves:
        player[0] = x
        player[1] = y
        return True

    return False

