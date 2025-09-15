#include <iostream>
#include <iomanip>

void showBalance(double balance);
double deposit();
double withdraw(double balance);


int main(){
    double balance = 123;
    int choice = 0;

    do{
        std::cout << "+------------------+\n";
        std::cout << " Enter your choice:\n";
        std::cout << " 1. Show Balance\n";
        std::cout << " 2. Deposit Money\n";
        std::cout << " 3. Withdraw Money\n";
        std::cout << " 4. Exit\n";
        std::cout << "+------------------+\n";
        std::cout << ">>> ";
        std::cin >> choice;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice)
        {
        case 1:
            showBalance(balance);  
            break;
        case 2:
            balance += deposit();
            showBalance(balance);
            break;
        case 3:
            balance -= withdraw(balance);
            showBalance(balance);
            break;
        case 4:
            std::cout << "Thanks for visiting!\n";
            break;
        
        default:
            std::cout << "Invalide choice\n";
            break;
        }
    }while(choice != 4);

    return 0;
}

void showBalance(double balance){
    std::cout << "Your balance is: " << std::setprecision(2) << std::fixed << balance << '$' << std::endl;
}
double deposit(){
    double sum;
    std::cout << "Please, write sum that you want to be deposited: ";
    std::cin >> sum;
    if (sum < 0){
        std::cout << "That's not a valide amount" << std::endl;
        return 0;
    }
    std::cout << "Thank you!" << std::endl;
    return sum;
}
double withdraw(double balance){
    double sum;
    std::cout << "Please, write sum that you want to withdraw: ";
    std::cin >> sum;
    if(balance < sum){
        std::cout << "Sorry, you don't have enough money\n";
        return 0;
    }
    if (sum < 0){
        std::cout << "That's not a valide amount" << std::endl;
        return 0;
    }
    std::cout << "Thank you!" << std::endl;
    return sum;
}
