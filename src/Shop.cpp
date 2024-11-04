#include "Shop.h"
#include "Buff.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

void runShop(Player& player, const std::vector<Buff>& buffs) {
    std::vector<std::pair<Buff, int>> availableBuffs;
    std::vector<bool> selected(buffs.size(), false);

    srand(time(0));

    // Select 5 random unique buffs from the list
    for (int i = 0; i < 5; ++i) {
        int index;
        do {
            index = rand() % buffs.size();
        } while (selected[index]);
        selected[index] = true;

        int price = 50 + rand() % 101;
        availableBuffs.emplace_back(buffs[index], price);
    }

    // Display available buffs and let player choose
    std::cout << "Welcome to the shop! Available buffs:\n\n";
    int num = 1;
    for (const auto& buff_pair : availableBuffs) {
        const auto& buff = buff_pair.first;
        int price = buff_pair.second;
        std::cout << "Name: " << buff.name << "\nEffect: " << buff.effect << "\nPrice: " << price << " gold\n";
    }


    int choice;
    std::cout << "Enter the number of the buff you want to buy (0 to exit): ";
    std::cin >> choice;

    if (choice > 0 && choice <= availableBuffs.size()) {
        int price = availableBuffs[choice - 1].second;
        if (player.gold >= price) {
            applyBuff(player, availableBuffs[choice - 1].first);
            player.gold -= price;
            std::cout << "You bought " << availableBuffs[choice - 1].first.name
                      << " for " << price << " gold!\n";
        } else {
            std::cout << "Not enough gold to buy this buff.\n";
        }
    } else if (choice == 0) {
        std::cout << "Exiting the shop.\n";
    } else {
        std::cout << "Invalid choice.\n";
    }
}
