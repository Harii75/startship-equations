#include "Combat.h"
#include "Utils.h" 
#include <iostream>
#include <cstdlib>
#include <ctime>

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

void fight(Player& player, Enemy& enemy) {
    while (player.ship->currentHealth > 0 && enemy.health > 0) {
        int num1, num2, playerAnswer, correctAnswer;
        char operation;

        // Generate a math equation to attack
        generateEquation(num1, num2, operation, correctAnswer);

        // Display equation to player
        std::cout << "\nSolve this equation to attack: " << num1 << " " << operation << " " << num2 << " = ";
        std::cin >> playerAnswer;

        // Calculate inflicted damage based on answer accuracy
        int inflictedDamage = player.calculateDamage(correctAnswer, playerAnswer);
        enemy.health -= inflictedDamage;
        if (enemy.health < 0) enemy.health = 0;

        // Show attack result and enemy's remaining health
        clearScreen();
        std::cout << "You dealt " << inflictedDamage << " damage to the enemy!";
        std::cout << " Enemy health: " << enemy.health << std::endl;

        // Enemy's turn to attack if still alive
        if (enemy.health > 0) {
            int enemyDamage = rand() % 10 + 5;  // Random enemy damage
            player.ship->currentHealth -= enemyDamage;
            loadingEffect(5, "\nEnemy's turn to attack!");
            std::cout << "\nThe enemy dealt " << enemyDamage << " damage to you!";
            std::cout << " Ship's health: " << player.ship->currentHealth << std::endl;
        }
    }

    // Outcome of the fight
    if (player.ship->currentHealth > 0) {
        std::cout << "\nYou've defeated the enemy!" << std::endl;
    } else {
        std::cout << "You were defeated by the enemy..." << std::endl;
    }
}