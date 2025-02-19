import random

def throw_the_dices() -> int:
    num = random.randint(1, 6)
    if num == 6:
        return throw_the_dices() + num
    return num


