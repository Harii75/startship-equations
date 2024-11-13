#include "MainMenu.h"
#include "Utils.h"
#include "FileLoader.h"
#include "ShipManager.h"
#include "InventoryManager.h"
#include "Stage.h"
#include "Weapon.h"
#include "Buff.h"
#include "GameProgression.h"
#include "Scenarios.h"
#include "ShipMenu.h"
#include "TradeMenu.h"
#include "Player.h"
#include "test.h"
#include <cassert>
#include <iostream>

void main_menu() {
    clearScreen();
    int choice = 0;
    do {
        clearScreen();
        std::cout << "\nMain Menu:\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Leaderboard\n";
        std::cout << "3. Test\n";
        std::cout << "4. Exit\n";
        std::cout << "\nChoose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                clearScreen();
                std::string player_name;
                std::cout << "\nEnter your name: ";
                std::cin >> player_name;
                while (player_name.length() > 20) {
                    std::cout << "\nName must be less than 20 characters. ";
                    std::cout << "\nEnter your name: ";
                    std::cin >> player_name;
                }
                Player player(player_name);
                game_menu(player);
                break;
            }
            case 2:
                clearScreen();
                std::cout << "Not available. Check Later\n";
                waitForKeypress();
                break;
            case 3:
                clearScreen();
                runTests();
                waitForKeypress();
                break;
            case 4:
                std::cout << "Exit...\n";
                break;
            default:
                clearScreen();
                std::cout << "Invalid input, try again.\n";
                break;
        }
    } while (choice != 3);
}

void game_menu(Player& player) {
    int choice = 0;

    // Load game data using FileLoader
    std::vector<Weapon> weapons = FileLoader::loadWeaponsFromXML("data/weapons.xml");
    std::vector<Stage> stages = FileLoader::loadStagesFromXML("data/levels.xml");
    std::vector<NPC> npcs = FileLoader::loadNpcsFromXML("data/npc.xml");
    std::vector<Buff> buffs = FileLoader::loadBuffsFromXML("data/buffs.xml");

    // Add random items to the player's inventory
    player.inventory->add_random_items(weapons);

    waitForKeypress();
    do {
        clearScreen();
        player.display_stats();
        std::cout << "\nGame Menu:\n";
        std::cout << "1. Continue Quest\n";
        std::cout << "2. Explore\n";
        std::cout << "3. Ship\n";
        std::cout << "4. Inventory\n";
        std::cout << "5. Trade\n";
        std::cout << "6. Exit\n";
        std::cout << "\nChoose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                clearScreen();
                std::cout << "The mission continues...\n";
                game_progression(player, stages, player.currentStage, player.currentLevel);
                waitForKeypress();
                break;
            case 2:
                clearScreen();
                std::cout << "Exploring the unknown..\n\n";
                randomScenario(player, weapons);
                waitForKeypress();
                break;
            case 3:
                std::cout << "Displaying ship menu...\n";
                clearScreen();
                ship_menu(player);
                break;
            case 4:
                clearScreen();
                std::cout << "Displaying inventory...\n\n";
                player.inventory->show_inventory();
                waitForKeypress();
                break;
            case 5:
                clearScreen();
                tradeMenu(player, buffs);
                break;
            case 6:
                clearScreen();
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice, please try again.\n";
                break;
        }
    } while (choice != 6);
}
