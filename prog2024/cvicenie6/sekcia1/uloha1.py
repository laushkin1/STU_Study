

def sucet_kladnych():
    r = 0
    while True:
        user_input = input(">>> ").strip()
        if not user_input.isdigit:
            break
        if int(user_input) <= 0:
            break
        r = r + int(user_input)
    return r


if __name__ == "__main__":
    print(sucet_kladnych())

