

def sucet_nacitanych(n: int) -> int:
    if n < 0:
        print("Argument must be positive")
        return -1
    elif n == 0: return 0

    print(f"Hello, please enter {n} numbers")
    while True:
        input_user = input(">>> ").strip().replace(" ", "").split(",")
        if len(input_user) != n:
            print(f"You enter wrong count of numbers\nPlease enter {n} numbers")
        else:
            r = 0
            for i in range(n):
                r+=int(input_user[i])
            break

    return r

if __name__ == "__main__":
    print(sucet_nacitanych(0))

