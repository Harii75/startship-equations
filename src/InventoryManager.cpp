#include "ShipManager.h"
#include "InventoryManager.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <numeric>
#include <chrono>

InventoryManager::InventoryManager() {}

void InventoryManager::add_item(const Weapon& weapon) {
    items.push_back(weapon);
    std::cout << weapon.name << " (" << weapon.rarity << ") added to inventory.\n";
}

void InventoryManager::show_inventory() {
    std::cout << "Inventory:\n";
    if (items.empty()) {
        std::cout << "Your inventory is empty.\n";
        waitForKeypress();
        clearScreen();
    } else {
        for (size_t i = 0; i < items.size(); ++i) {
            const auto& weapon = items[i];
            std::cout << i << ": " << weapon.name << " (" << weapon.rarity << ") - (+" << weapon.damage << ")\n";
        }
    }
}

bool InventoryManager::can_trade() {
    return !items.empty();
}

void InventoryManager::buy_item(const Weapon& weapon) {
    items.push_back(weapon);
    std::cout << weapon.name << " bought.\n";
}

void InventoryManager::sell_item(const std::string& item) {
    auto it = std::remove_if(items.begin(), items.end(),
        [&](const Weapon& weapon) { return weapon.name == item; });
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
        add_item(weapon);
    }
}

void InventoryManager::remove_item(const std::string& weaponName) {
    auto it = std::find_if(items.begin(), items.end(), [&](const Weapon& w) { return w.name == weaponName; });
    if (it != items.end()) {
        items.erase(it);
    }
}

bool InventoryManager::equip_weapon_by_index(size_t index, int& playerDamage) {
    if (index >= items.size()) {
        clearScreen();
        std::cout << "Please equip an existing item or enter -1 to cancel.\n";
        waitForKeypress();
        return false;
    }

    if (hasEquippedWeapon) {
        clearScreen();
        std::cout << "Unequip the current weapon first.\n";
        waitForKeypress();
        return false;
    }

    equippedWeapon = items[index];
    playerDamage += equippedWeapon.damage;
    hasEquippedWeapon = true;
    items.erase(items.begin() + index);
    clearScreen();
    loadingEffect(3,"Equipping weapon");
    std::cout << "\n";
    std::cout << equippedWeapon.name << " equipped. New damage: " << playerDamage - equippedWeapon.damage << " --> " << playerDamage << "\n";
    waitForKeypress();
    return true;
}

bool InventoryManager::unequip_weapon(int& playerDamage) {
    if (!hasEquippedWeapon) {
        clearScreen();
        std::cout << "No weapon is currently equipped.\n";
        waitForKeypress();
        return false;
    }

    std::cout << "\nAre you sure you want to unequip " << equippedWeapon.name << "? [Y/N]: ";
    char confirm;
    std::cin >> confirm;
    if (confirm != 'Y' && confirm != 'y') {
        clearScreen();
        std::cout << "Unequip canceled.\n";
        waitForKeypress();
        return false;
    }
    clearScreen();
    loadingEffect(3,"Unequipping weapon");
    std::cout << "\n";
    playerDamage -= equippedWeapon.damage;
    add_item(equippedWeapon);
    hasEquippedWeapon = false;    
    std::cout << equippedWeapon.name << " unequipped. New damage: " << playerDamage + equippedWeapon.damage << " --> " << playerDamage << "\n";
    equippedWeapon = Weapon(); 
    waitForKeypress();
    return true;
}


void InventoryManager::list_inventory() {
    show_inventory();
}

std::vector<Weapon> InventoryManager::filterWeaponsByRarity(const std::vector<Weapon>& weapons, const std::string& rarity) {
    std::vector<Weapon> filteredWeapons;
    std::copy_if(weapons.begin(), weapons.end(), std::back_inserter(filteredWeapons),
                 [&rarity](const Weapon& weapon) {
                     return weapon.rarity == rarity;
                 });
    return filteredWeapons;
}

Weapon InventoryManager::getRandomWeapon(const std::vector<Weapon>& filteredWeapons) {
    if (filteredWeapons.empty()) {
        throw std::runtime_error("No weapons available for the specified rarity.");
    }
    std::mt19937 gen(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<> dis(0, filteredWeapons.size() - 1);
    return filteredWeapons[dis(gen)];
}