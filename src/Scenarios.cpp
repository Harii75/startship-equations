#include "Scenarios.h"
#include "Utils.h"
#include "Combat.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

void randomScenario(Player& player, const std::vector<Weapon>& weapons) {    
    std::srand(static_cast<unsigned int>(std::time(0)));
    int scenario = std::rand() % 4 + 1;

    switch (scenario) {
        case 1:
            std::cout << "Nothing happens as you drift through space.\n";
            std::cout << "You've gained 3 knowledge points.\n";
            player.knowledge += 3;
            break;
        case 2: {
            int freeGold = std::rand() % 101;
            player.gold += freeGold;
            std::cout << "You found a floating cabinet and collected " << freeGold << " gold!\n";
            std::cout << "You've gained 3 knowledge points.\n";
            player.knowledge += 3;
            break;
        }
        case 3:
            std::cout << "An asteroid collision! Solve these equations to stabilize your ship:\n";
            asteroidCollisionChallenge(player);
            break;
        case 4: {
            std::cout << "You encounter an enemy bandit ship!\n";
            Enemy darkShip;
            darkShip.damage = 10 * player.currentStage + 1;
            darkShip.health = 100 * (player.currentStage + 1) / 2;
            darkShip.type = "Bandits";

            fight(player, darkShip);
            std::cout << "\nThat was a great fight with a lot of experiences gained.";
            std::cout << "\nYou've gained 6 knowledge points.\n";
            player.knowledge += 6;
            break;
        }
        default:
            std::cout << "Unknown scenario.\n";
            break;
    }
}
