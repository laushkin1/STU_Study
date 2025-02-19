

def druhe_najvacsie(n: int) -> int|None:
    if n < 2:
        print("Argument must minimum 2")
        return None

    print(f"Hello, please enter {n} numbers")
    while True:
        input_user = [int(i) for i in input(">>> ").strip().replace(" ", "").split(",")]
        input_user.sort()
        if len(input_user) != n:
            print(f"You enter wrong count of numbers\nPlease enter {n} numbers")
        else:
            break

    return input_user[n-2]

if __name__ == "__main__":
    print(druhe_najvacsie(3))

