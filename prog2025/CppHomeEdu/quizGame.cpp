#include <iostream>

int main(){

    std::string questions[] = {
                                "1. What year was C++ created?: ",
                                "2. Who invented C++?: ",
                                "3. What is the predecessor of C++?: ",
                                "4. Is the Earth flat?: "
    };

    std::string option[][4] = {
                                {"A. 1969", "B. 1975", "C. 1985", "D. 1989"},
                                {"A. Guido van Rossum", "B. Bjarne Stroustrup", "C. John Carmack", "D. Mark Zuckerburg"},
                                {"A. C", "B. C+", "C. C--", "D. 1C"},
                                {"A. Yes", "B. No", "C. Sometimes", "D. What's Earth?"},
    };

    char answerKey[] = {'C', 'B', 'A', 'B'};

    int size = sizeof(questions)/sizeof(questions[0]);
    char guess;
    int score = 0;

    for(int i = 0; i < size; i++){
        std::cout << "**************************************" << std::endl;
        std::cout << questions[i] << std::endl;
        std::cout << "**************************************" << std::endl;

        for(int j = 0; j < sizeof(option[i])/sizeof(option[i][0]); j++){
            std::cout << option[i][j] << std::endl; 
        }

        std::cin >> guess;
        guess = toupper(guess);

        if(guess == answerKey[i]){
            std::cout << "CORRECT!" << std::endl;
            score++;
        }
        else{
            std::cout << "WRONG" << std::endl;
            std::cout << "Answer: " << answerKey[i] << std::endl;
        }
    }

    std::cout << "**************************************" << std::endl;
    std::cout << "*               RESULT               *" << std::endl;
    std::cout << "**************************************" << std::endl;
    std::cout << "CORRECT GUESSES: " << score << std::endl;
    std::cout << "# of QUESTIONS: " << size << std::endl;
    std::cout << "SCORE: " << ((float)score/size)*100 << '%' << std::endl;


    return 0;
}