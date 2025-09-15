#include <iostream>
#include <ctime>

void drawBoard(char *spaces);
void playerMove(char *spaces, char player);
void computerMove(char *spaces, char computer);
bool checkWinner(char *spaces, char player, char computer);
bool checkTie(char *spaces);

int main(){
    char spaces[9]; for(int i = 1; i < 10; i++) spaces[i-1] = i+'0';
    char player = 'X';
    char computer = 'O';
    bool running = true;

    drawBoard(spaces);
    while(running){
        playerMove(spaces, player);
        drawBoard(spaces);
        if(checkWinner(spaces, player, computer)){
            running = false;
            break;
        }
        
        computerMove(spaces, computer);
        drawBoard(spaces);
        if(checkWinner(spaces, player, computer)){
            running = false;
            break;
        }
    }

    return 0;
}


void drawBoard(char *spaces){
    std::cout << std::endl;
    std::cout << "******************" << std::endl;
    std::cout << "     |     |     " << std::endl;
    std::cout << "  " << spaces[0] << "  |  " << spaces[1] << "  |  " << spaces[2] << "  " << std::endl;
    std::cout << "_____|_____|_____" << std::endl;
    std::cout << "     |     |     " << std::endl;
    std::cout << "  " << spaces[3] << "  |  " << spaces[4] << "  |  " << spaces[5] << "  " << std::endl;
    std::cout << "_____|_____|_____" << std::endl;
    std::cout << "     |     |     " << std::endl;
    std::cout << "  " << spaces[6] << "  |  " << spaces[7] << "  |  " << spaces[8] << "  " << std::endl;
    std::cout << "     |     |     " << std::endl;
    std::cout << "******************" << std::endl;
    std::cout << std::endl;
}

void playerMove(char *spaces, char player){
    int number;
    do{
        std::cout << "Enter a spot to place a marker (1-9)" << std::endl;
        std::cout << ">>> ";
        std::cin >> number;
        number--;
        if(number > 8) continue;
        if(number < 0) continue;
        if(spaces[number] != 'X' && spaces[number] != 'O'){
            spaces[number] = player;
            break;
        }
    }while(1);
}

void computerMove(char *spaces, char computer){
    int number;
    srand(time(0));
    while(1){
        number = rand() % 9;
        if(spaces[number] != 'X' && spaces[number] != 'O'){
            spaces[number] = computer;
            break;
        }
    }
    std::cout << "COMPUTER: " << number+1 << std::endl;
}

bool checkWinner(char *spaces, char player, char computer){
    for(int i = 0, j = 0; i<9 && j<3; i+=3, j++){
        if(spaces[i] == spaces[i+1] && spaces[i+1] == spaces[i+2]){
            spaces[i] == player ? std::cout << "PLAYER WON!" << std::endl : std::cout << "COMPUTER WON!" << std::endl;
            return true;
        }
        if(spaces[j] == spaces[j+3] && spaces[j+3] == spaces[j+6]){
            spaces[j] == player ? std::cout << "PLAYER WON!" << std::endl : std::cout << "COMPUTER WON!" << std::endl;
            return true;
        }
        if((spaces[4] == spaces[2] && spaces[4] == spaces[6]) || (spaces[4] == spaces[0] && spaces[4] == spaces[8])){
            spaces[4] == player ? std::cout << "PLAYER WON!" << std::endl : std::cout << "COMPUTER WON!" << std::endl;
            return true;
        }
    }
    if(checkTie(spaces)){
        std::cout << "IT'S TIE!" << std::endl;
        return true;
    }
    return false;
}

bool checkTie(char *spaces){
    for(int i = 0; i < 9; i++){
        if(spaces[i] != 'X' && spaces[i] != 'O') return false;
    }
    return true;
}
