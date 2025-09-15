#include <iostream>
#include <ctime>

void make_square(int side);

int main(){
    srand(time(NULL));
    int num = (rand() % 6) + 1;
    std::cout << num << std::endl;
    make_square(num);
    return 0;
}

void make_square(int side){
    if(side <= 0) return;
    std::cout << '+' << ' ';
    for(int i = 0; i < side; i++)
        std::cout << '-' << ' ';
    std::cout << '+';
    std::cout << '\n';

    for(int j = 0; j < side; j++){
        std::cout << '|' << ' ';
        for(int i = 0; i < side; i++)
            std::cout << ' ' << ' ';
        std::cout << '|';

        std::cout << '\n';
    }

    std::cout << '+' << ' ';
    for(int i = 0; i < side; i++)
        std::cout << '-' << ' ';
    std::cout << '+';
    std::cout << '\n';
}