#ifndef WEAPON_H
#define WEAPON_H

#include <string>

struct Weapon {
    std::string name;
    int damage;
    std::string rarity;

    // Constructor with parameters
    Weapon(const std::string& name, int damage, const std::string& rarity)
        : name(name), damage(damage), rarity(rarity) {}

    // Default constructor
    Weapon() : name(""), damage(0), rarity("") {}
};


#endif 
