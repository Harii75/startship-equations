#include "Challenges.h"
#include "Utils.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <conio.h>

void asteroidCollisionChallenge(Player& player) {
    int goodAnswer = 0;
    std::cout << "\nYou are flying through an asteroid field, your left wing has hit an asteroid.\nComplete this 3 challange to stabilize you ship!\n";
    std::cout << "\nShip's health: " << player.ship->currentHealth << "\n"; 
    for (int i = 0; i < 3; ++i) {
        waitForKeypress();
        int num1 = rand() % 10 + 1;
        int num2 = rand() % 10 + 1;
        int correctAnswer = num1 * num2;
        clearScreen();
        std::cout << "You have to recover you ship, solve this equation to steer: " << num1 << " * " << num2 << " = ";

        auto start = std::chrono::steady_clock::now();
        std::string answer; 
        bool answeredInTime = false;

        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(5)) {
            if (_kbhit()) {
                char ch = _getch();
                if (ch == '\r') break;
                answer += ch;
                answeredInTime = true;
            }
        }

        if (!answeredInTime || answer.empty()) {
            std::cout << "\nYou could not avoid this asteroid.\nPart of your ship is badly damaged. (-30)!\n";
            player.ship->currentHealth -= 30;
            std::cout << "\nRemaining ship health is: "<< player.ship->currentHealth; 
        } else {
            int userAnswer = std::stoi(answer);
            if (userAnswer == correctAnswer) {
                goodAnswer += 1;
                std::cout << "\nAsteroid avoided! Stay focused!.\n";
            } else {
                std::cout << "\nIncorrect. The answer was: " << correctAnswer << ". You failed to steer your ship! (-15).\n";
                player.ship->currentHealth -= 15;
                std::cout << "\nRemaining ship health is: "<< player.ship->currentHealth; 
            }
        }

        if (player.ship->currentHealth <= 0) {
            std::cout << "\nYour couldn't avoid the asteroids. \nYour ship has fallen to pieces.\n";
            //endgame function
            break;
        }
    }

    waitForKeypress();
    clearScreen();
    switch(goodAnswer) {
        case 0:
            std::cout << "Somehow you made it. Unbelivable...";
            break;
        case 1:
            std::cout << "You did you best, your left wing fully fell off.";
            break;
        case 2:
            std::cout << "Well done! Survived the asteroid collosion with minimal damage.";
            break;
        default:
            std::cout << "Perfect steering captain! You saved the day!";
            break;
    }
    std::cout << "\nChallenge complete. Player health: " << player.ship->currentHealth << std::endl;
    std::cout << "\nYou've earned " << (goodAnswer*4) <<" knowledge points.\n";
    player.knowledge += goodAnswer*4;
}