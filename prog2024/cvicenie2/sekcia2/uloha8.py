
def postupny_sucet_geometrickeho_radu(a0:int, q:int, N:int) -> None:
    print(a0)
    a = a0
    for _ in range(1, N):
        a0 = a0 * q
        a = a + a0
        print(a)

if __name__ == "__main__":
    # postupny_sucet_geometrickeho_radu(1,2,5)
    postupny_sucet_geometrickeho_radu(1,0.5,4)

