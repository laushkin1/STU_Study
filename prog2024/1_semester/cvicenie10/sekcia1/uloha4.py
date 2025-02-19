
def sucet_matic(A: [list], B: [list]) -> [list]:
    cols = len(A)
    rows = len(A[0])

    if cols != len(B) or rows != len(B[0]):
        print("Zadane matice nie je mozne scitať")
        return None

    C = []
    for c in range(cols):
        temp = []
        for r in range(rows):
            temp.append(A[c][r] + B[c][r])
        C.append(temp)
        #C.append([A[c][r] + B[c][r] for r in range(rows)])

    return C


if __name__ == "__main__":
    A = [
            [1, 2],
            [3, 5]
        ]

    B = [
            [5, 2],
            [-3, 4]
        ]

    print(sucet_matic(A, B))
    print(sucet_matic(A, [[5, 2], [-3, 4], [1, 1]]))

