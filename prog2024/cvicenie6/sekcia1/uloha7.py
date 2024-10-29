
def najdlhsia_podpostupnost() -> int:
    num = None
    counter = 1
    counter_max = 0
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

        if num == None:
            num = input_user
        elif num == input_user:
            counter += 1
        else:
            if counter >= counter_max:
                counter_max = counter
            counter = 1

        if input_user == 0: break
        num = input_user

    return counter_max

if __name__ == "__main__":
    print(najdlhsia_podpostupnost())

