
def priemer_troch(a:int, b:int, c:int) -> float:
    print(round(((a+b+c)/3), 2))

if __name__ == "__main__":
    priemer_troch(1, 2, 3) # 2.0
    priemer_troch(1, 2, 4.5) # 2.5

