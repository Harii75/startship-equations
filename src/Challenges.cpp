#include "Challenges.h"
#include "HighScore.h"
#include "Utils.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <iostream>
#ifdef _WIN32
    #include <conio.h> 
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

namespace CrossPlatformInput {
    // Cross-platform kbhit implementation
    bool kbhit() {
    #ifdef _WIN32
        return _kbhit();
    #else
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);          
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);       
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF) {
            ungetc(ch, stdin);
            return true;
        }

        return false;
    #endif
    }

    // Cross-platform getch implementation
    char getch() {
    #ifdef _WIN32
        return _getch();
    #else
        struct termios oldt, newt;
        char ch;

        tcgetattr(STDIN_FILENO, &oldt);         
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);        
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); 

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
        return ch;
    #endif
    }

} 


void asteroidCollisionChallenge(Player& player) {
    int goodAnswer = 0;
    std::cout << "\nYou are flying through an asteroid field, your left wing has hit an asteroid.\nComplete these 3 challenges to stabilize your ship!\n";
    std::cout << "\nShip's health: " << player.ship->currentHealth << "\n"; 

    for (int i = 0; i < 3; ++i) {
        waitForKeypress();
        int num1 = rand() % 10 + 1;
        int num2 = rand() % 10 + 1;
        int correctAnswer = num1 * num2;
        clearScreen();
        std::cout << "You have to recover your ship, solve this equation to steer: " << num1 << " * " << num2 << " = ";

        auto start = std::chrono::steady_clock::now();
        std::string answer; 
        bool answeredInTime = false;

        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(5)) {
            if (kbhit()) {
                char ch = getch();
                if (ch == '\r') break; // On Linux, replace '\r' with '\n' if necessary
                answer += ch;
                answeredInTime = true;
            }
        }

        if (!answeredInTime || answer.empty()) {
            std::cout << "\nYou could not avoid this asteroid.\nPart of your ship is badly damaged. (-30)!\n";
            player.ship->currentHealth -= 30;
            std::cout << "\nRemaining ship health is: " << player.ship->currentHealth; 
        } else {
            int userAnswer = std::stoi(answer);
            if (userAnswer == correctAnswer) {
                goodAnswer += 1;
                std::cout << "\nAsteroid avoided! Stay focused!.\n";
            } else {
                std::cout << "\nIncorrect. The answer was: " << correctAnswer << ". You failed to steer your ship! (-15).\n";
                player.ship->currentHealth -= 15;
                std::cout << "\nRemaining ship health is: " << player.ship->currentHealth; 
            }
        }

        if (player.ship->currentHealth <= 0) {
            std::cout << "\nYou couldn't avoid the asteroids.\nYour ship has fallen to pieces.\n";
            addHighScore(player.name, player.highscore);
            endGame(player);
            return;
        }
    }

    waitForKeypress();
    clearScreen();
    switch (goodAnswer) {
        case 0:
            std::cout << "Somehow you made it. Unbelievable...";
            break;
        case 1:
            std::cout << "You did your best, but your left wing fully fell off.";
            player.highscore += 5;
            break;
        case 2:
            std::cout << "Well done! You survived the asteroid collision with minimal damage.";
            player.highscore += 10;
            break;
        default:
            std::cout << "Perfect steering, Captain! You saved the day!";
            player.highscore += 20;
            break;
    }
    std::cout << "\nChallenge complete. Player health: " << player.ship->currentHealth << std::endl;
    std::cout << "\nYou've earned " << (goodAnswer * 4) << " knowledge points.\n";
    player.knowledge += goodAnswer * 4;
}
