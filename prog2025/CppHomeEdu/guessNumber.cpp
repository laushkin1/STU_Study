#include <iostream>
#include <ctime>

int generate_random_number(int from = 1, int to = 1){
    return (rand() % (to-from+1)) + from;
}

int main(){
    srand(time(0));
    int f = 0, t = 227;
    int secret_num = generate_random_number(f, t);
    int user_number;
    int steps = 0;

    std::cout << "Guess random number between " << f << " and " << t << " included!" << std::endl;

    do{
        std::cout << ">>> ";
        std::cin >> user_number;
        (user_number > secret_num) ? std::cout << "It's too big number\n" : std::cout << "It's too little number\n";
        steps++;
    }while(user_number != secret_num);

    std::cout << "Congratulation, you win in " << steps << " steps!" << std::endl;

    return 0;
}
