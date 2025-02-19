
def priemer_troch(a:int, b:int, c:int) -> float:
    return round(((a+b+c)/3), 2)

if __name__ == "__main__":
    print(priemer_troch(1, 2, 3)) # 2.0
    print(priemer_troch(1, 2, 4.5)) # 2.5

