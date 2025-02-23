
def any_lowercase(s: str) -> bool:
    for i in s:
        if i.islower():
            return True

    return False

if __name__ == "__main__":
    print(any_lowercase("anna"))
    print(any_lowercase("ANNA"))
    print(any_lowercase("ANNa"))
    print(any_lowercase(""))

