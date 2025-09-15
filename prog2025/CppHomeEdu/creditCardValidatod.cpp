#include <iostream>
#include <vector>

bool is_valid(const std::string& card);
int sumOddDigits(const std::string& card);
int sumEvenDigits(const std::string& card);
int getDigit(const int number);
std::string printCard(const std::string& card);

int main(){

    std::vector<std::string> cards = {
        "6011000990139424", // 1
        "4539578763621486", // 1
        "6011000990139424", // 1
        "5500005555555559", // 1
        "4111111111111112", // 0
        "5105105105105100", // 1
        "3782822463100050", // 0
        "1234567890123456"  // 0
    };

    for (std::string card : cards){
        if(is_valid(card))
            std::cout << '\'' << printCard(card) << '\'' << " is valid" << std::endl;
        else
            std::cout << '\'' << printCard(card) << '\'' << " is not valid" << std::endl;

    }
    
    return 0;
}

bool is_valid(const std::string& card){
    if(card.size() != 16) return false;
    return ((sumOddDigits(card) + sumEvenDigits(card)) % 10 == 0);
}

int sumOddDigits(const std::string& card){
    int sum = 0;
    for(int i = 14; i>=0; i-=2){
        sum += getDigit((card[i] - '0') * 2);
    }
    return sum;
}

int sumEvenDigits(const std::string& card){
    int sum = 0;
    for(int i = 15; i>=0; i-=2){
        sum += getDigit(card[i] - '0');
    }
    return sum;
}

int getDigit(const int number){
    return ((number % 10) + (number / 10 % 10));
}

std::string printCard(const std::string& card) {
    std::string res;
    res.reserve(card.size() + card.size() / 4);

    for (size_t i = 0; i < card.size(); i++) {
        res.push_back(card[i]);
        if ((i + 1) % 4 == 0 && i + 1 < card.size()) {
            res.push_back(' ');
        }
    }

    return res;
}
