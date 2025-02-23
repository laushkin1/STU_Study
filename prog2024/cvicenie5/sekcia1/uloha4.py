
def parita_suctu_rekurzivne(n: int) -> bool:
    if n <= 0: return 1

    input_user = int(input(">>> "))
    return not bool((input_user + parita_suctu_rekurzivne(n-1))%2)


if __name__ == "__main__":
    print(parita_suctu_rekurzivne(4))

