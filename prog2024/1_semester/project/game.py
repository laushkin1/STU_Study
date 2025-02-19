from map import create_map, print_map
from player import move_player, can_move_teleport
from dice import throw_the_dices


def my_input(start: int, end: int) -> int:
    # my input to control valid of data
    num = input(">>> ")
    if num.isdigit():
        num = int(num)
        if start <= num <= end:
            return num

    return -1


def win(our_map: [list], player: list) -> bool:
    # func to check if somebody win
    map_len = len(our_map)

    for x in range(map_len):
        for y in range(map_len):
            if our_map[player[0]][player[1]] == '*':
                return True

    return False


def play() -> None:

    # Enter the entry parameters
    print("Please enter size of the map")
    map_size = my_input(5, 10)
    if map_size == -1:
        print("Sorry, you enter wrong number!")
        return None

    print("Enter number of players")
    players_count = my_input(1, 4)
    if players_count == -1:
        print("Sorry, you enter wrong number!")
        return None

    # Create map
    our_map = create_map(map_size)

    # Create players (coordinates of players)
    players = []
    for i in range(players_count):
        players.append([0, 0])


    # print first map without players
    print_map(our_map)
    while True:

        for i in range(players_count):
            print(f"Positions of players:")

            # print position of all players
            for j in range(players_count):
                print(f"Player {j+1} {players[j]}")
            print("-"*(players_count+1))

            # print the moves that the player received and check whether these moves are allowed
            moves = throw_the_dices()
            print(f"Player {i+1} throw the dices and got {moves} moves")
            allow_move = move_player(players[i], moves, our_map)
            if allow_move:
                print(f"Player {i+1} go to {players[i]} position")

                # check if teleportation allow and if allow teleport
                teleportation = can_move_teleport(players[i], our_map)
                if teleportation == 1:
                    print(f"Player {i+1} go to {players[i]} through the positive teleport")
                elif teleportation == -1:
                    print(f"Player {i+1} go to {players[i]} through the negative teleport")

            else:
                # if move not allow print it
                print(f"Player {i+1} got more moves than needed to finish!")

            # print map to show new position of players
            print_map(our_map, players)

            # check whether the player who made the move won
            if win(our_map, players[i]):
                print(f"Player {i+1} WON!")
                return


if __name__ == "__main__":
    play()

