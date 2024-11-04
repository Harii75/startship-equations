#include "TradeMenu.h"
#include "Utils.h"
#include "Shop.h"
#include <iostream>

void tradeMenu(Player& player, const std::vector<Buff>& buffs) {
    player.canPlayerTrade();

    if (player.canTrade) {
        int choice;
        std::cout << "Welcome to the market!\n";
        std::cout << "1. Sell\n";
        std::cout << "2. Buy\n";
        std::cout << "3. Exit\n";
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                player.inventory->show_inventory();
                waitForKeypress();
                break;
            case 2:
                runShop(player, buffs);
                break;
            case 3:
                std::cout << "Exiting market.\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    } else {
        std::cout << "You are not in a trading area.\n";
        waitForKeypress();
    }
}
