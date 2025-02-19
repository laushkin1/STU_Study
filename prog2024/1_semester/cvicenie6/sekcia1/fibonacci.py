
def fibonacci(n: int) -> int:
    if n == 0: return 1
    if n == 1: return 1
    return fibonacci(n-1) + fibonacci(n-2)


if __name__ == "__main__":
    print(fibonacci(0))
    print(fibonacci(1))
    print(fibonacci(2))
    print(fibonacci(3))
    print(fibonacci(4))
    print(fibonacci(5))

