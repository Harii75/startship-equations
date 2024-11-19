#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include <string>
#include <vector>
#include <utility>
#include "Weapon.h"
#include "ShipManager.h"

class InventoryManager {
public:
    std::vector<Weapon> items;
    Weapon equippedWeapon;
    bool hasEquippedWeapon = false;

    InventoryManager();
    void show_inventory();
    bool can_trade();
    void buy_item(const Weapon& weapon);
    void sell_item(const std::string& item);
    void add_random_items(const std::vector<Weapon>& weapons, int count = 2);
    void add_item(const Weapon& weapon);
    void remove_item(const std::string& weaponName);
    bool equip_weapon_by_index(size_t index, int& playerDamage);
    bool unequip_weapon(int& playerDamage);
    void list_inventory();
    const Weapon& get_item(size_t index);
    std::vector<Weapon> filterWeaponsByRarity(const std::vector<Weapon>& weapons, const std::string& rarity);
    Weapon getRandomWeapon(const std::vector<Weapon>& filteredWeapons);
};

#endif