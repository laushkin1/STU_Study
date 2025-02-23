

def pocet_delitelnych(n: int, k: int) -> int:
    if n <= 0 or k <= 0:
        print("All arguments must be positive")
        return -1

    print(f"Hello, please enter {n} numbers")
    while True:
        input_user = input(">>> ").strip().replace(" ", "").split(",")
        if len(input_user) != n:
            print(f"You enter wrong count of numbers\nPlease enter {n} numbers")
        else:
            r = 0
            for i in range(n):
                if not int(input_user[i])%k: r+=1
            break

    return r

if __name__ == "__main__":
    print(pocet_delitelnych(-1, -2))

