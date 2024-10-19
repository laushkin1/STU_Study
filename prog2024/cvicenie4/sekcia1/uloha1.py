
def test_parity(n: int) -> bool:
    if(n%2): return False
    return True

if __name__ == "__main__":
    print(test_parity(2))
    print(test_parity(0))
    print(test_parity(5))
    print(test_parity(13))

