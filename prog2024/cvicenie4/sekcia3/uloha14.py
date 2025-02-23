
def rovnaka_farba(x1: int, y1: int, x2: int, y2: int) -> bool:
    if not all(i < 9 and i > 0 for i in [x1, x2, y1, y2]):
        return False

    chess_board = []
    for i in range(8):
        if i%2: chess_board.append([1, 0, 1, 0, 1, 0, 1, 0])
        else: chess_board.append([0, 1, 0, 1, 0, 1, 0, 1])

    indx_x1 = 8-x1
    indx_x2 = 8-x2
    indx_y1 = y1-1
    indx_y2 = y2-1
    if chess_board[indx_y1][indx_x1] == chess_board[indx_y2][indx_x2]: return True
    return False

if __name__ == "__main__":
    print(rovnaka_farba(4, 2, 3, 7))
    print(rovnaka_farba(4, 2, 7, 6))
    print(rovnaka_farba(1, 1, 8, 8))
    print(rovnaka_farba(8, 1, 1, 7))
    print(rovnaka_farba(8, 1, 1, 9))
    print(rovnaka_farba(8, 1, 0, 8))

