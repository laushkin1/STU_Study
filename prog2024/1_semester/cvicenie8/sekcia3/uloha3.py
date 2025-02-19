
def if_a_mirror(d_age: int, m_age: int):
    return bool(str(d_age).zfill(2)[::-1] == str(m_age).zfill(2))


def find_age() -> tuple | None:

    mother_age_list = []
    daughter_age_list = []

    for a in range(28):
        count = 0
        for i in range(83):
            d = i
            m = d+18+a
            if if_a_mirror(d, m):
                count += 1
                mother_age_list.append(m)
                daughter_age_list.append(d)

            if count == 8: return (mother_age_list, daughter_age_list)

    return None


def find_d_age(ages: tuple) -> int | None:
    if ages == None: return None

    mother_age_list, daughter_age_list = ages

    return daughter_age_list[5]



if __name__ == "__main__":
    print(f"Daughter is {find_d_age(find_age())} years old!")

