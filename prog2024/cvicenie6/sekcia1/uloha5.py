
def vacsi_naslednik() -> int:
    num = None
    counter = 0
    while True:

        while True:
            neg = False
            input_user = input(">>> ").strip()
            if input_user.startswith('-'):
                neg = True
                input_user = input_user[1:]

            if input_user.isdigit():
                input_user = int(input_user)
                if neg:
                    input_user -= input_user*2
                    neg = False
                break
            print("Please, enter a number")

        if num != None:
            if num <= input_user:
                counter+=1

        num = input_user

        if input_user == 0: break

    return counter


if __name__ == "__main__":
    print(vacsi_naslednik())

