
#def je_palindrom(ret: str) -> bool:
#    if len(ret)%2:
#        if ret[:int((len(ret))/2)] == ret[int((len(ret)+1)/2):][::-1]:
#            return True
#    else:
#        if ret[:int(len(ret)/2)] == ret[int(len(ret)/2):][::-1]:
#            return True
#    return False


def je_palindrom(ret: str) -> bool:
    if ret == ret[::-1]: return True
    return False

if __name__ == "__main__":
    print(je_palindrom("anna"))
    print(je_palindrom("anka"))
    print(je_palindrom("kobylamamalybok"))
    print(je_palindrom(""))

