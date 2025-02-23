
def od_jedna_po_n(n: int) -> None:
    for i in range(n):
        [print(f"{j+1} ", end="") for j in range(i+1)]
        print("\n", end="")

if __name__ == "__main__":
    od_jedna_po_n(5)

