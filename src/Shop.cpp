#include "Shop.h"
#include "Buff.h"
#include "Player.h"  
#include "Utils.h"
#include <vector>
#include <functional>
#include <iostream>
#include <limits>


std::vector<std::pair<Buff, int>> shopInventory;

int calculateBuffPrice(const Buff& buff) {
    std::hash<std::string> hash_fn;
    size_t hash = hash_fn(buff.name);
    int basePrice = 50;
    return basePrice + (hash % 101);  // Price range: 50 to 150
}

void resetShopInventory(const std::vector<Buff>& buffs) {
    shopInventory.clear();
    std::vector<bool> selected(buffs.size(), false);
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < 3 && i < buffs.size(); ++i) {
        int index;
        do {
            index = rand() % buffs.size();
        } while (selected[index]);
        selected[index] = true;

        int price = calculateBuffPrice(buffs[index]);
        shopInventory.emplace_back(buffs[index], price);
    }
}

void runShop(Player& player, const std::vector<Buff>& buffs) {
    clearScreen();
    resetShopInventory(buffs);

    // Check if the player has exceeded the visit limit
    if (player.visitCounter >= 2) {
        std::cout << "The shop is closed. Come back later!\n";
        return;
    }

    // Increment the visit counter each time the player enters the shop
    player.visitCounter++;

    int choice;
    while (true) {
        clearScreen();
        // Check if the shop is sold out
        if (shopInventory.empty()) {
            std::cout << "The shop is sold out. Come back later!\n";
            break;
        }

        // Display available buffs with descriptions and prices
        std::cout << "\nWelcome to the shop! Available buffs:\n\n";
        for (size_t i = 0; i < shopInventory.size(); ++i) {
            const auto& buff = shopInventory[i].first;
            int price = shopInventory[i].second;
            std::cout << i + 1 << ". " << buff.name << "\n";
            std::cout << "   Description: " << buff.effect << "\n";
            std::cout << "   Price: " << price << " gold\n\n";
        }

        std::cout << "Enter the number of the buff you want to buy (0 to exit): ";
        std::cin >> choice;
        clearScreen();
        if (choice == 0) {
            std::cout << "Exiting the shop.\n";
            break;
        } else if (choice > 0 && choice <= shopInventory.size()) {
            int price = shopInventory[choice - 1].second;
            if (player.gold >= price) {
                player.gold -= price;  // Deduct gold
                Buff purchasedBuff = shopInventory[choice - 1].first;
                player.addPurchasedBuff(purchasedBuff);  // Add buff to player's purchased buffs
                std::cout << "You bought " << purchasedBuff.name << " for " << price << " gold!\n";
                waitForKeypress();
                shopInventory.erase(shopInventory.begin() + (choice - 1));  // Remove item from shop
            } else {
                std::cout << "Not enough gold to buy this buff.\n";
                waitForKeypress();
            }
        } else {
            std::cout << "Invalid choice.\n";
            waitForKeypress();
        }
    }
}
