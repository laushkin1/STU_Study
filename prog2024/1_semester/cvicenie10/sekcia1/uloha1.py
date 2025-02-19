
def najviac_nacitane() -> int:
    user_numbers = []
    max_num = 0

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

        if len(user_numbers) >= 1:
            old_max_num = max(set(user_numbers), key=user_numbers.count)
            omnk = user_numbers.count(old_max_num)

            user_numbers.append(input_user)

            new_max_num = max(set(user_numbers), key=user_numbers.count)
            nmnk = user_numbers.count(new_max_num)

            if old_max_num != new_max_num:
                if nmnk > omnk: max_num = new_max_num
                elif nmnk < omnk: max_num = old_max_num
        else:
            user_numbers.append(input_user)

        if input_user == 0: break


    return max_num

if __name__ == "__main__":
    print(f"Most common num is: {najviac_nacitane()}")

