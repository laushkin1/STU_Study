
def maximum_rekurzivne(n: int) -> int:
    if n <= 0: return -9999

    input_user = int(input(">>> "))
    return max(input_user, maximum_rekurzivne(n-1))


if __name__ == "__main__":
    print(maximum_rekurzivne(4))

