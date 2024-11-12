#ifndef SHIP_MANAGER_H
#define SHIP_MANAGER_H

#include <iostream>
#include "ShipManager.h"
#include "InventoryManager.h"

class ShipManager {
public:
    int storage_capacity;
    int maxHealth;
    int currentHealth;
    int damage;

    ShipManager();

    void upgrade_ship();
    void repair_ship();
    void show_info();

};

#endif 
