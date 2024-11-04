#include "Scenarios.h"
#include "Utils.h"
#include "Combat.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

void randomScenario(Player& player, const std::vector<Weapon>& weapons) {
    player.knowledge += 5;
    std::srand(static_cast<unsigned int>(std::time(0)));

    int scenario = std::rand() % 5 + 1;

    switch (scenario) {
        case 1:
            std::cout << "Nothing happens as you drift through space.\n";
            break;
        case 2: {
            int freeGold = std::rand() % 101;
            player.gold += freeGold;
            std::cout << "You found a floating cabinet and collected " << freeGold << " gold!\n";
            break;
        }
        case 3:
            std::cout << "An asteroid collision! Solve these equations to stabilize your ship:\n";
            asteroidCollisionChallenge(player);
            break;
        case 4: {
            std::cout << "You encounter an enemy ship!\n";
            Enemy darkShip;
            darkShip.damage = 10 * player.currentStage + 1;
            darkShip.health = (100 * player.currentStage + 1) / 2;
            darkShip.type = "Stealth Ship";
            fight(player, darkShip);
            player.knowledge += 5;
            break;
        }
        case 5:
            std::cout << "An unexpected event occurs in space.\n";
            break;
        default:
            std::cout << "Unknown scenario.\n";
            break;
    }
}
