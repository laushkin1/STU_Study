
def je_anagram(retazec1: str, retazec2: str) -> bool:
    retazec1_list = list(retazec1)
    retazec2_list = list(retazec2)
    retazec1_list.sort()
    retazec2_list.sort()
    return retazec1_list == retazec2_list

if __name__ == "__main__":
    print(je_anagram("silent", "listen"))
    print(je_anagram("skola", "sloka"))
    print(je_anagram("skola", "laska"))

