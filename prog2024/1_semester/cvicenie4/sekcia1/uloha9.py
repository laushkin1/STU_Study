

def minimum(n: int) -> int|None:
    if n <= 0:
        print("Argument must be positive")
        return None

    print(f"Hello, please enter {n} numbers")
    while True:
        input_user = [int(i) for i in input(">>> ").strip().replace(" ", "").split(",")]
        input_user.sort()
        if len(input_user) != n:
            print(f"You enter wrong count of numbers\nPlease enter {n} numbers")
        else:
            break

    return input_user[0]

if __name__ == "__main__":
    print(minimum(4))

