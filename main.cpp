#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "tinyxml2.h"


class Player;
void main_menu();
void game_menu(Player& player);

class ShipManager {
public:
    int storage_capacity;
    int health;
    int damage;

    ShipManager() : storage_capacity(100), health(100), damage(50) {}

    void upgrade_ship() {
        health += 25;
        std::cout << "Ship expanded! New capacity: " << health << "\n";
    }

    void repair_ship() {
        health = 100;
        std::cout << "Ship fully repaired!\n";
    }

    void equip_weapon(int weapon_damage) {
        damage += weapon_damage;
        std::cout << "New weapon equipped! Damage: " << damage << "\n";
    }

    void show_info() {
        std::cout << "Ship info:\n";
        std::cout << " - Capacity: " << storage_capacity << "\n";
        std::cout << " - Health: " << health << "\n";
        std::cout << " - Damage: " << damage << "\n";
    }
};

class InventoryManager {
public:
    std::vector<std::pair<std::string, std::string>> items; // Pair of item name and type

    InventoryManager() {
        // Initial items with types
        items.emplace_back("Laser Cannon", "weapon");
        items.emplace_back("Shield Booster", "buff");
        items.emplace_back("Energy Cells", "resource");
    }

    void add_item(const std::string& item, const std::string& type) {
        items.emplace_back(item, type);
        std::cout << item << " added to inventory.\n";
    }

    void show_inventory() {
        std::cout << "Inventory:\n";
        if (items.empty()) {
            std::cout << "Your inventory is empty.\n";
        } else {
            for (const auto& item : items) {
                std::cout << "- " << item.first << " (" << item.second << ")\n";
            }
        }
    }

    bool can_trade() {
        return !items.empty();
    }

    void buy_item(const std::string& item, const std::string& type) {
        items.emplace_back(item, type);
        std::cout << item << " bought.\n";
    }

    void sell_item(const std::string& item) {
        auto it = std::remove_if(items.begin(), items.end(),
            [&](const std::pair<std::string, std::string>& p) { return p.first == item; });
        if (it != items.end()) {
            std::cout << item << " sold.\n";
            items.erase(it, items.end());
        } else {
            std::cout << item << " cannot be found in inventory.\n";
        }
    }
};

class Player {
public:
    std::string name;
    int gold;
    int level;
    int xp;
    int knowledge;
    ShipManager* ship;
    InventoryManager* inventory;

    Player(const std::string& name) : name(name), gold(500), level(1), xp(0), knowledge(0) {
        ship = new ShipManager();
        inventory = new InventoryManager();
    }

    void display_stats() {
        std::cout << "######################################################\n";
        std::cout << "#                 Captain: " << name << "             #\n";
        std::cout << "#   Gold: " << gold << "   Level: " << level << "   XP: " << xp << "/240  #\n";
        std::cout << "#   Knowledge: " << knowledge << "                               #\n";
        std::cout << "######################################################\n";
    }

    ~Player() {
        delete ship;
        delete inventory;
    }
};

// Main menu options
void main_menu() {
    int choice = 0;
    do {
        std::cout << "\nMain Menu:\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Leaderboard\n";
        std::cout << "3. Exit\n";
        std::cout << "\nChoose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string player_name;
                std::cout << "\nEnter your name: ";
                std::cin >> player_name;
                Player player(player_name);
                game_menu(player);
                break;
            }
            case 2:
                std::cout << "Not available. Check Later\n";
                break;
            case 3:
                std::cout << "Exit...\n";
                break;
            default:
                std::cout << "Invalid input, try again.\n";
                break;
        }
    } while (choice != 3);
}

bool load_weapons(Player& player) {
    tinyxml2::XMLDocument doc;

    // Load the XML file
    if (doc.LoadFile("data/weapons.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return false;  // Return false if loading fails
    }

    // Find the root element (assumed to be <weapons>)
    tinyxml2::XMLElement* weaponsElement = doc.FirstChildElement("weapons");
    if (!weaponsElement) {
        std::cerr << "No <weapons> element found in the XML file." << std::endl;
        return false;  // Return false if no <weapons> element is found
    }

    // Iterate through each <weapon> element
    tinyxml2::XMLElement* weaponElement = weaponsElement->FirstChildElement("weapon");
    while (weaponElement) {
        const char* name = weaponElement->FirstChildElement("name")->GetText();
        int damage = weaponElement->FirstChildElement("damage")->IntText();
        const char* rarity = weaponElement->FirstChildElement("rarity")->GetText();

        // Print weapon details
        std::cout << "Weapon: " << (name ? name : "Unknown")
                  << ", Damage: " << damage
                  << ", Rarity: " << (rarity ? rarity : "Unknown")
                  << std::endl;

        // Add the weapon to the player's inventory
        player.inventory->add_item(name, "weapon");

        weaponElement = weaponElement->NextSiblingElement("weapon");
    }

    return true;  // Return true if loading was successful
}

bool load_buffs(Player& player) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("data/buffs.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return false;
    }

    tinyxml2::XMLElement* buffsElement = doc.FirstChildElement("buffs");
    if (!buffsElement) {
        std::cerr << "No <buffs> element found in the XML file." << std::endl;
        return false;
    }

    tinyxml2::XMLElement* buffElement = buffsElement->FirstChildElement("buff");
    while (buffElement) {
        const char* name = buffElement->FirstChildElement("name")->GetText();
        const char* effect = buffElement->FirstChildElement("effect")->GetText();
        int duration = buffElement->FirstChildElement("duration")->IntText();

        std::cout << "Buff: " << (name ? name : "Unknown")
                  << ", Effect: " << (effect ? effect : "Unknown")
                  << ", Duration: " << duration << " seconds" << std::endl;

        // Here you can add buffs to the player or ship as needed
        buffElement = buffElement->NextSiblingElement("buff");
    }

    return true;
}

bool load_levels(Player& player) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("data/levels.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return false;
    }

    tinyxml2::XMLElement* levelsElement = doc.FirstChildElement("levels");
    if (!levelsElement) {
        std::cerr << "No <levels> element found in the XML file." << std::endl;
        return false;
    }

    tinyxml2::XMLElement* levelElement = levelsElement->FirstChildElement("level");
    while (levelElement) {
        int level_number = levelElement->FirstChildElement("level_number")->IntText();
        int xp_required = levelElement->FirstChildElement("xp_required")->IntText();
        const char* reward = levelElement->FirstChildElement("reward")->GetText();

        std::cout << "Level: " << level_number
                  << ", XP Required: " << xp_required
                  << ", Reward: " << (reward ? reward : "Unknown") << std::endl;

        // Here you can check if the player qualifies for the next level
        levelElement = levelElement->NextSiblingElement("level");
    }

    return true;
}

bool load_npcs(Player& player) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("data/npcs.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return false;
    }

    tinyxml2::XMLElement* npcsElement = doc.FirstChildElement("npcs");
    if (!npcsElement) {
        std::cerr << "No <npcs> element found in the XML file." << std::endl;
        return false;
    }

    tinyxml2::XMLElement* npcElement = npcsElement->FirstChildElement("npc");
    while (npcElement) {
        const char* id = npcElement->FirstChildElement("id")->GetText();
        const char* stage = npcElement->FirstChildElement("stage")->GetText();
        const char* name = npcElement->FirstChildElement("name")->GetText();
        const char* quest = npcElement->FirstChildElement("quest")->GetText();
        const char* reward = npcElement->FirstChildElement("reward")->GetText();

        std::cout << "NPC ID: " << (id ? id : "Unknown")
                  << ", Stage: " << (stage ? stage : "Unknown")
                  << ", Name: " << (name ? name : "Unknown")
                  << ", Quest: " << (quest ? quest : "Unknown")
                  << ", Reward: " << (reward ? reward : "Unknown") << std::endl;

        // Here you can interact with NPCs as needed
        npcElement = npcElement->NextSiblingElement("npc");
    }

    return true;
}

// Function to drop a random weapon from the player's inventory
void drop_random_weapon(Player& player) {
    // Ensure the inventory is not empty
    if (player.inventory->items.empty()) {
        std::cout << "No weapons available to drop.\n";
        return;
    }

    // Randomly select a weapon from the player's inventory
    int random_index = rand() % player.inventory->items.size();
    std::string dropped_weapon = player.inventory->items[random_index].first;

    // Add the dropped weapon to the player's inventory
    std::cout << "You dropped a random weapon: " << dropped_weapon << "\n";
    player.inventory->sell_item(dropped_weapon); // Optionally remove it from inventory
}

// In-game menu options
void game_menu(Player& player) {
    int choice = 0;

    //Load Weapons
    bool success = load_weapons(player);
    if (success) {
        std::cout << "\nWeapons loaded successfully!\n";
    } else {
        std::cout << "\nFailed to load weapons from XML file.\n";
    }

    // Load buffs
    success = load_buffs(player);
    if (success) {
        std::cout << "\nBuffs loaded successfully!\n";
    } else {
        std::cout << "\nFailed to load buffs from XML file.\n";
    }

    // Load levels
    success = load_levels(player);
    if (success) {
        std::cout << "\nLevels loaded successfully!\n";
    } else {
        std::cout << "\nFailed to load levels from XML file.\n";
    }

    // Load NPCs
    success = load_npcs(player);
    if (success) {
        std::cout << "\nNPCs loaded successfully!\n";
    } else {
        std::cout << "\nFailed to load NPCs from XML file.\n";
    }

    do {
        player.display_stats();
        player.inventory->show_inventory();
        player.ship->show_info();

        std::cout << "\nGame Menu:\n";
        std::cout << "1. Continue Quest\n";
        std::cout << "2. Ship Info\n";
        std::cout << "3. Inventory\n";
        std::cout << "4. Trade\n";
        std::cout << "5. Explore\n";
        std::cout << "6. Exit\n";
        std::cout << "\nChoose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "The mission continues...\n";
                break;
            case 2:
                std::cout << "Displaying ship info...\n";
                player.ship->show_info();
                break;
            case 3:
                std::cout << "Displaying inventory...\n";
                player.inventory->show_inventory();
                break;
            case 4:
                if (player.inventory->can_trade()) {
                    std::string item;
                    std::cout << "What would you like to sell? ";
                    std::cin >> item;
                    player.inventory->sell_item(item);
                } else {
                    std::cout << "You have no items to sell.\n";
                }
                break;
            case 5:
            {
                int random_event = rand() % 3;
                switch (random_event) {
                    case 0:
                        std::cout << "You found a treasure chest!\n";
                        break;
                    case 1:
                        std::cout << "You found some resources!\n";
                        break;
                    case 2:
                        std::cout << "You encountered a mysterious object!\n";
                        drop_random_weapon(player); // Call the function to drop a random weapon
                        break;
                }
                break;
            }
            case 6:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice, please try again.\n";
                break;
        }
    } while (choice != 6);
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
    main_menu();
    return 0;
}
