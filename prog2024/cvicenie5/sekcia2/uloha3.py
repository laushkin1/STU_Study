
def dec_to_bin(n: int) -> None:
    if n > 1:
        dec_to_bin(n//2)
    print(n%2, end="")

if __name__ == "__main__":
    dec_to_bin(10)
    print()
    dec_to_bin(13)
    print()
    dec_to_bin(0)
    print()
    dec_to_bin(31)
    print()

