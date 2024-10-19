

def minimum_dvoch(a: int, b: int) -> int:
    return a if a<=b else b

if __name__ == "__main__":
    print(minimum_dvoch(2, 5))
    print(minimum_dvoch(5, 5))
    print(minimum_dvoch(-2, -103))

