
def fibonacci(n: int) -> int:
    if n == 0: return 1
    elif n == 1: return 1
    else:
        predposledny = 1
        posledny = 1
        aktualny = predposledny + posledny
        for i in range(n-2):
            predposledny = posledny
            posledny = aktualny
            aktualny = predposledny + posledny
        return aktualny

def is_perfect_sqrt(number: int) -> bool:
    s = int(number**0.5)
    return s*s == number

def fibonacci_index(x: int) -> int:
    if x == 1: return 0
    if not (is_perfect_sqrt(5*x*x + 4) or is_perfect_sqrt(5*x*x - 4)): return -1

    counter = 0
    while True:
        counter += 1
        if counter == 99999:
            counter = -2
            break
        elif fibonacci(counter) == x: break

    return counter

if __name__ == "__main__":
#    print(fibonacci(0))
#    print(fibonacci(1))
#    print(fibonacci(2))
#    print(fibonacci(3))
#    print(fibonacci(4))
#    print(fibonacci(5))

    print(fibonacci_index(1))
    print(fibonacci_index(2))
    print(fibonacci_index(3))
    print(fibonacci_index(4))
    print(fibonacci_index(5))
    print(fibonacci_index(6))
    print(fibonacci_index(7))
    print(fibonacci_index(8))

