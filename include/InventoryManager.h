#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include <string>
#include <vector>
#include <utility>
#include "Weapon.h"

class InventoryManager {
public:
    std::vector<std::pair<std::string, std::string>> items;

    InventoryManager();

    void add_item(const std::string& item, const std::string& type);
    void show_inventory();
    bool can_trade();
    void buy_item(const std::string& item, const std::string& type);
    void sell_item(const std::string& item);
    void add_random_items(const std::vector<Weapon>& weapons, int count = 2);
};

#endif 
