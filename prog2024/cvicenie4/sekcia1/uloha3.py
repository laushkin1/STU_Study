
def minimum_troch(a: int, b: int, c: int) -> int:
    if a<=b and a<=c: return a
    return b if b<=a and b<=c else c

if __name__ == "__main__":
    print(minimum_troch(2, 5, -2))
    print(minimum_troch(5, 1, 10))
    print(minimum_troch(-105, 1, -105))
