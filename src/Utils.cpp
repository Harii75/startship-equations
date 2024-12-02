#include "Utils.h"
#include "HighScore.h"
#include <iostream>
#include <ctime> 

#ifdef _WIN32
    #include <conio.h> 
#else
    #include <termios.h>
    #include <unistd.h>
#endif

void waitForKeypress() {
    std::cout << "\nPress any button to continue...";
    std::cout.flush(); 
    

#ifdef _WIN32
    _getch();
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); 
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); 

    getchar(); 

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
#endif
}
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void loadingEffect(int durationInSeconds, const std::string& message) {
    std::cout << message;
    for (int i = 0; i < durationInSeconds; ++i) {
        std::cout << ".";
        std::cout.flush();
        clock_t end_time = clock() + CLOCKS_PER_SEC;
        while (clock() < end_time) {}
    }
}


void finishGame(Player& player){
    clearScreen();    
    int choice;
    std::cout << "You have successfully cleared the space! You're score: " << player.highscore << "\n\n";
    std::cout << "You can check the highscores or exit the game.\n";
    std::cout << "\n1. Highscores";
    std::cout << "\n2. Exit\n";

    std::cin >> choice;
    switch(choice){
        case 1:
            clearScreen();
            displayTop10HighScores();
            waitForKeypress();
            clearScreen();
            endGame(player);
            break;
        case 2:
            clearScreen(); 
            exit(0);
            break;
        default:
            clearScreen();
            std::cout << "Invalid input.";
            endGame(player); 
            break;
    };
}

void endGame(Player& player){    
    clearScreen();
    int choice;
    std::cout << "You have died! Your score: " << player.highscore << "\n\n";
    std::cout << "You can check the highscores or exit the game.\n";
    std::cout << "\n1. Highscores";
    std::cout << "\n2. Exit\n";

    std::cin >> choice;
    switch(choice){
        case 1:
            clearScreen();
            displayTop10HighScores();
            waitForKeypress();
            clearScreen();
            endGame(player);
            break;
        case 2:
            clearScreen(); 
            exit(0);
            break;
        default:
            clearScreen();
            std::cout << "Invalid input.";
            endGame(player); 
            break;
    };
}