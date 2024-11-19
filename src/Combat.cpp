#include "Combat.h"
#include "Utils.h" 
#include "HighScore.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>

void generateEquation(int &num1, int &num2, char &operation, int &correctAnswer) {
    num1 = rand() % 20 + 1;
    num2 = rand() % 20 + 1;
    int op = rand() % 2;  // 0 for addition, 1 for subtraction

    if (op == 0) {
        operation = '+';
        correctAnswer = num1 + num2;
    } else {
        operation = '-';
        correctAnswer = num1 - num2;
    }
}

double calculatePercentageDifference(double num1, double num2) {
    double difference = std::abs(num1 - num2);
    double percentageDifference = (difference / std::abs(num1)) * 100.0;

    return percentageDifference;
}

void fight(Player& player, Enemy& enemy) {
    int perfect = 0;
    int mid = 0;
    int bad = 0;

    while (player.ship->currentHealth > 0 && enemy.health > 0) {
        int overallPercentage = 0;
        int num1, num2, playerAnswer, correctAnswer;
        char operation;

        // Generate a math equation to attack
        generateEquation(num1, num2, operation, correctAnswer);

        bool correctInput = false;

        // Loop until the player provides a valid input
        do {
            clearScreen();
            std::cout << "\nSolve this equation to attack: " << num1 << " " << operation << " " << num2 << " = ";

            if (!(std::cin >> playerAnswer)) {
                std::cin.clear(); // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                std::cout << "Invalid input. Please enter a valid number.\n";
            } else {
                correctInput = true; // Input is valid, break the loop
            }
        } while (!correctInput);

        // Calculate inflicted damage based on answer accuracy
        int inflictedDamage = player.calculateDamage(correctAnswer, playerAnswer);
        enemy.health -= inflictedDamage;

        if (inflictedDamage > enemy.health){
            player.knowledge += 5;
        }

        overallPercentage = std::ceil(calculatePercentageDifference(playerAnswer, correctAnswer));

        // Update high score based on accuracy
        if (overallPercentage >= 0 && overallPercentage <= 25) {
            player.highscore += 15;
            perfect += 1;
        } else if (overallPercentage >= 26 && overallPercentage <= 50) {
            player.highscore += 10;
            mid += 1;
        } else if (overallPercentage >= 51 && overallPercentage <= 100) {
            player.highscore += 5;
            bad += 1;
        } else if (overallPercentage >= 101 && overallPercentage <= 150) {
            player.highscore += 2;
        }

        

        if (enemy.health < 0) {
            enemy.health = 0;
            if (player.ship->currentHealth == player.ship->maxHealth) {
                player.highscore += 25;
            }
        }

        // Show attack result and enemy's remaining health
        std::cout << "You dealt " << inflictedDamage << " damage to the enemy!";
        std::cout << " Enemy health: " << enemy.health << std::endl;

        // Enemy's turn to attack if still alive
        if (enemy.health > 0) {
            std::srand(std::time(0));
            int enemyDamage = (std::rand() % enemy.damage) + 1;
            player.ship->currentHealth -= enemyDamage;
            loadingEffect(5, "\nEnemy's turn to attack!");
            std::cout << "\nThe enemy dealt " << enemyDamage << " damage to you!";
            std::cout << " Ship's health: " << player.ship->currentHealth << std::endl;
        }
    }

    // Outcome of the fight
    if (player.ship->currentHealth > 0) {
        std::cout << "\nYou've defeated the enemy!" << std::endl;
        int overallKnowledge = (perfect*3)+(mid*2)+(bad*1);
        player.knowledge += overallKnowledge;
    } else {
        std::cout << "You were defeated by the enemy..." << std::endl;
        addHighScore(player.name, player.highscore);
        endGame(player);
    }
}