#include <iostream>

int main()
{
    std::cout << "Hello, this is my c++ calculator!" << '\n';
    std::cout << "You can use follow operators" << '\n';
    std::cout << "\" + - * / = \"" << '\n';
    std::cout << "Just enter num, press enter and after that type symbol" << '\n';
    std::cout << " " << '\n';

    bool run = true;
    bool divide_err = false;

    double num = 1;
    std::cout << "Number: ";
    std::cin >> num;

    double result = num;

    char symbols = '+';

    while (run && !divide_err)
    {
        std::cout << "Operator: ";
        std::cin >> symbols;

        if (symbols == '=') break;

        std::cout << "Number: ";
        std::cin >> num;
        
        switch (symbols)
        {
        case '+': result += num; break;
        case '-': result -= num; break;
        case '*': result *= num; break;
        case '/':
            if (num == 0)
            {
                std::cout << "Divide by zero error" << '\n';
                divide_err = true;
            }else result /= num;
            break;
        default: std::cout << "Unknown operator!\n"; break;
        }
    }

    if (!divide_err) std::cout << "Result: " << result << '\n';

    return 0;
}
