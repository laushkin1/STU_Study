
def index_najvacsieho() -> int:
    indx = -1
    indx_max = -1
    num_max = None
    while True:

        while True:
            neg = False
            input_user = input(">>> ").strip()
            if input_user.startswith('-'):
                neg = True
                input_user = input_user[1:]

            if input_user.isdigit():
                input_user = int(input_user)
                indx += 1
                if neg:
                    input_user -= input_user*2
                    neg = False
                break
            print("Please, enter a number")

        if num_max == None or num_max <= input_user:
            num_max = input_user
            indx_max = indx

        if input_user == 0: break

    return indx_max if indx_max >=0 else 0

if __name__ == "__main__":
    print(index_najvacsieho())
