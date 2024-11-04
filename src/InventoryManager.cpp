#include "InventoryManager.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

InventoryManager::InventoryManager() {}

void InventoryManager::add_item(const std::string& item, const std::string& type) {
    items.emplace_back(item, type);
    std::cout << item << " (" << type << ") added to inventory.\n";
}

void InventoryManager::show_inventory() {
    std::cout << "Inventory:\n";
    if (items.empty()) {
        std::cout << "Your inventory is empty.\n";
    } else {
        for (const auto& item : items) {
            std::cout << "- " << item.first << " (" << item.second << ")\n";
        }
    }
}

bool InventoryManager::can_trade() {
    return !items.empty();
}

void InventoryManager::buy_item(const std::string& item, const std::string& type) {
    items.emplace_back(item, type);
    std::cout << item << " bought.\n";
}

void InventoryManager::sell_item(const std::string& item) {
    auto it = std::remove_if(items.begin(), items.end(),
        [&](const std::pair<std::string, std::string>& p) { return p.first == item; });
    if (it != items.end()) {
        std::cout << item << " sold.\n";
        items.erase(it, items.end());
    } else {
        std::cout << item << " cannot be found in inventory.\n";
    }
}

void InventoryManager::add_random_items(const std::vector<Weapon>& weapons, int count) {
    std::mt19937 gen(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

    int itemsToAdd = std::min(count, static_cast<int>(weapons.size()));

    std::vector<int> indices(weapons.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), gen);

    for (int i = 0; i < itemsToAdd; ++i) {
        const Weapon& weapon = weapons[indices[i]];
        add_item(weapon.name, weapon.rarity);
    }
}
