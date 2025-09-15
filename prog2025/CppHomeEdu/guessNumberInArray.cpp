#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>

// This is mini game on guessing the numbers

int searchNum(const std::vector<int> &array, int num);
std::vector<int> create_random_array();
void print(std::string text);

int main(){

    srand(time(0));

    print("Hello user, i am computer and i want to play with you\n");
    print("Now i created a random array of numbers\n");
    print("Your mission guessing at least one number\n");
    print("An array will be of 1-100 elements\n");
    print("Eatch element will be number from 1 till 100\n");
    print("\n");
    print("Good luck!\n");
    print("\n");
    
    std::vector<int> numbers = create_random_array();
    std::cout << "Enter a number for search:\n";
    int myNum = -1;
    int answer;
    int steps = 0;
    do{
        steps++;
        std::cout << ">>> ";
        std::cin >> myNum;
        answer = searchNum(numbers, myNum);
        if (answer < 0) std::cout << "There is no such number in the array" << std::endl;
        else break;
    }while(1);

    print("Congratilation, you win in ");
    std::cout << steps;
    print(" steps\n");

    print("Your number ");
    std::cout << myNum;
    print(" is on the ");
    std::cout << answer;
    print(" possition\n");

    print("Here is my array:\n");
    for(int number : numbers)
        std::cout << number << ' ';
    std::cout << std::endl;

    return 0;
}

int searchNum(const std::vector<int> &array, int num){
    for(int i = 0; i<array.size(); i++){
        if(array[i] == num) return i;
    }
    return -1;
}

std::vector<int> create_random_array(){
    std::vector<int> res;
    for(int i = 0; i < ((rand() % 100)+1); i++)
        res.push_back(((rand() % 100)+1));
    return res;
}

void print(std::string text){
    for(int i = 0; i < text.size(); i++){
        std::cout << text[i] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }
}
