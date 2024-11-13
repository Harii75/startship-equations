#include "TradeMenu.h"
#include "Utils.h"
#include "Shop.h"
#include <iostream>

int calculateSellPrice(const Weapon& weapon) {
    int basePrice = weapon.damage * 10; 
    float rarityMultiplier;

    if (weapon.rarity == "Common") rarityMultiplier = 1.0;
    else if (weapon.rarity == "Uncommon") rarityMultiplier = 1.5;
    else if (weapon.rarity == "Rare") rarityMultiplier = 2.0;
    else if (weapon.rarity == "Epic") rarityMultiplier = 3.0;
    else if (weapon.rarity == "Legendary") rarityMultiplier = 5.0;

    return static_cast<int>(basePrice * rarityMultiplier);
}

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
                clearScreen();
                // Display inventory with selling prices
                if (player.inventory->can_trade()) {
                    for (size_t i = 0; i < player.inventory->items.size(); ++i) {
                        const Weapon& weapon = player.inventory->items[i];
                        int sellPrice = calculateSellPrice(weapon);
                        std::cout << i + 1 << ". " << weapon.name << " ("
                                  << weapon.rarity << ", Damage: " << weapon.damage
                                  << ") - Sell Price: " << sellPrice << "\n";
                    }

                    std::cout << "\nEnter the number of the item to sell (0 to cancel): ";
                    int itemIndex;
                    std::cin >> itemIndex;

                    // Allow the user to exit by entering 0
                    if (itemIndex == 0) {
                        clearScreen();
                        std::cout << "\nSell cancelled. Returning to market menu.\n";
                    } 
                    else if (itemIndex > 0 && itemIndex <= player.inventory->items.size()) {
                        Weapon weaponToSell = player.inventory->items[itemIndex - 1];
                        int sellPrice = calculateSellPrice(weaponToSell);

                        player.gold += sellPrice;  // Add gold to player
                        player.inventory->remove_item(weaponToSell.name);  // Remove item from inventory

                        std::cout << "You sold " << weaponToSell.name << " for " << sellPrice << " gold.\n";
                    } else {
                        std::cout << "Invalid selection.\n";
                    }
                } else {
                    std::cout << "Your inventory is empty.\n";
                }
                waitForKeypress();
                break;
            case 2:
                clearScreen();
                runShop(player, buffs);
                waitForKeypress();
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
