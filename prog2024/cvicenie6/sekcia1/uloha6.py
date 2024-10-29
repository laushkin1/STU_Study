
def pocet_maxim() -> int:
    num_max = None
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

        if num_max == None or num_max < input_user:
            num_max = input_user
            counter=0
        if num_max == input_user and num_max != None:
            counter+=1

        if input_user == 0: break

    return counter


if __name__ == "__main__":
    print(pocet_maxim())

