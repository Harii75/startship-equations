#include "ShipMenu.h"
#include "Utils.h"
#include <iostream>

void ship_menu(Player& player) {
    int upgradeCost = 150;
    int repairCost = 100;
    int choice = 0;

    do {
        std::cout << "\nShip Menu:\n";
        std::cout << "1. Ship information \n";
        std::cout << "2. Upgrade ship\n";
        std::cout << "3. Repair ship\n";
        std::cout << "4. Equip weapon\n";
        std::cout << "5. Exit ship menu\n";
        std::cout << "\nChoose an option: ";
        std::cin >> choice;
        clearScreen();
        switch (choice) {
            case 1:
                std::cout << "Displaying ship information...\n\n";
                player.ship->show_info();
                std::cout << "\n";
                waitForKeypress();
                clearScreen();
                break;
            case 2: {
                std::cout << "Would you like to upgrade your ship? (150 gold) [Y/N]: ";
                std::string answer;
                std::cin >> answer;

                if (answer == "Y" || answer == "y") {
                    if (player.gold >= upgradeCost) {
                        player.gold -= upgradeCost;
                        loadingEffect(5,"\nUpgrading your ship");
                        player.ship->upgrade_ship();
                        std::cout << "Remaining gold: " << player.gold << "\n";
                        waitForKeypress();
                    } else {
                        std::cout << "\nYou don't have enough gold (150 required).\n";
                    }
                }
                
                waitForKeypress();
                clearScreen();
                break;
            }

            case 3: {
                std::string answer;
                if (player.ship->maxHealth == player.ship->currentHealth){
                    std::cout << "Your ship is already fully repaired.";
                } else {
                    std::cout << "Would you like to repair your ship? (100 gold) [Y/N]: ";
                    std::cin >> answer;
                    if (answer == "Y" || answer == "y") {
                        if (player.gold >= repairCost) {
                            player.gold -= repairCost;
                            loadingEffect(5,"\nRepairing your ship");
                            player.ship->repair_ship();
                            std::cout << "Remaining gold: " << player.gold << "\n";
                            waitForKeypress();
                        } else {
                            std::cout << "\nYou don't have enough gold (100 required).\n";
                        }
                    }
                }
                waitForKeypress();
                clearScreen();
                break;
            }

            case 4:
                break;

            case 5:
                std::cout << "Exiting...\n";
                clearScreen();
                break;

            default:
                std::cout << "Invalid choice, please try again.\n";
                clearScreen();
                break;
        }
    } while (choice != 5);
}