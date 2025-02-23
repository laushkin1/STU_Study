
def pocet_parnych_rekurzivne(n: int) -> int:
    if n <= 0: return 0

    while True:
        input_user = input(">>> ").strip()

        if not input_user.isdigit(): print("Please enter num!")
        else: break

    if not int(input_user)%2:
        return 1 + pocet_parnych_rekurzivne(n-1)
    return pocet_parnych_rekurzivne(n-1)

if __name__ == "__main__":
    print(pocet_parnych_rekurzivne(5))

