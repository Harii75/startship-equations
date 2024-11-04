#include "ShipManager.h"
#include <iostream>

ShipManager::ShipManager() : storage_capacity(100), currentHealth(100), damage(50), maxHealth(100) {}

void ShipManager::upgrade_ship() {
    maxHealth += 25;
    currentHealth += 25;
    std::cout << "\n\nShip upgraded! New health: " << currentHealth << "/" << maxHealth << "\n";
}

void ShipManager::repair_ship() {
    currentHealth = maxHealth;
    std::cout << "\n\nShip fully repaired!\n";
}

void ShipManager::equip_weapon(int weapon_damage) {
    damage += weapon_damage;
    std::cout << "\n\nNew weapon equipped! Damage: " << damage << "\n";
}

void ShipManager::show_info() {
    std::cout << "Ship info:\n";
    std::cout << " - Capacity: " << storage_capacity << "\n";
    std::cout << " - Health: " << currentHealth << "/" << maxHealth << "\n";
    std::cout << " - Damage: " << damage << "\n";
}
