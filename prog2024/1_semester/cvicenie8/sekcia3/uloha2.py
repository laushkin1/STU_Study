def adapter(num: int) -> str:
    num_res = list(str(num).zfill(6))

    if len(num_res) > 6:
        num_res = list(str(num-1000000).zfill(6))


    for i in range(len(num_res)-1):
        num_res[i] = num_res[i] + '-'

    return ''.join(num_res)


def unadapter(odometr: str) -> int:
    return int(odometr.replace('-', ''))


def je_palindrom(ret: str) -> bool:
   if ret == ret[::-1]: return True
   return False


def plus_km_to_odometr(odometr: str, km: int) -> str:
    return adapter(unadapter(odometr)+km)



def find_km(odometr: str) -> bool:
    last_4 = odometr[4:]
    if not je_palindrom(last_4): return False

    last_5 = plus_km_to_odometr(odometr, 1)[2:]
    if not je_palindrom(last_5): return False

    average_4 = plus_km_to_odometr(odometr, 2)[2:-2]
    if not je_palindrom(average_4): return False

    all_odometr = plus_km_to_odometr(odometr, 3)
    if not je_palindrom(all_odometr): return False

    return True


if __name__ == "__main__":
    for i in range(999999+1):
        if find_km(adapter(i)):
            print(adapter(i))

