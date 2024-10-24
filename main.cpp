#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // Include this for std::find
#include <cstdlib>

class Player;
class ShipManager;
class InventoryManager;

// Function prototypes
void main_menu();
void game_menu(Player& player);

class ShipManager {
public:
    int storage_capacity;
    int health;
    int damage;

    ShipManager() : storage_capacity(100), health(100), damage(50) {}

    void upgrade_ship() {
        storage_capacity += 50;
        std::cout << "Ship expanded! New capacity: " << storage_capacity << "\n";
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
        std::cout << " - Damaga: " << damage << "\n";
    }
};

class InventoryManager {
public:
    std::vector<std::string> items;

    void add_item(const std::string& item) {
        items.push_back(item);
        std::cout << item << " added to inventory.\n";
    }

    void show_inventory() {
        std::cout << "Inventory:\n";
        for (const auto& item : items) {
            std::cout << "- " << item << "\n";
        }
    }

    bool can_trade() {
        return !items.empty();
    }

    void buy_item(const std::string& item) {
        items.push_back(item);
        std::cout << item << " bought.\n";
    }

    void sell_item(const std::string& item) {
        auto it = std::find(items.begin(), items.end(), item);
        if (it != items.end()) {
            std::cout << item << " sold.\n";
            items.erase(it);
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
    ShipManager* ship;
    InventoryManager* inventory;

    Player(const std::string& name) : name(name), gold(500), level(1), xp(0) {
        ship = new ShipManager();
        inventory = new InventoryManager();
    }

    void display_stats() {
        std::cout << "######################################################\n";
        std::cout << "#                 Captain: " << name << "             #\n";
        std::cout << "#   Gold: " << gold << "   Level: " << level << "   XP: " << xp << "/240  #\n";
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

// In-game menu options
void game_menu(Player& player) {
    int choice = 0;
    do {
        std::cout << "\nGame Menu:\n";
        std::cout << "1. Continue Quest\n";
        std::cout << "2. Ship Info\n";
        std::cout << "3. Inventory\n";
        std::cout << "4. Trade\n";
        std::cout << "5. Exit\n";
        std::cout << "\nChoose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "The mission continues...\n";
                player.display_stats();
                break;
            case 2:
                player.ship->show_info();
                break;
            case 3:
                player.inventory->show_inventory();
                break;
            case 4:
                if (player.inventory->can_trade()) {
                    std::string item;
                    std::cout << "What would you like to sell? ";
                    std::cin >> item;
                    player.inventory->sell_item(item);
                } else {
                    std::cout << "You have no item to sell.\n";
                }
                break;
            case 5:
                std::cout << "Exiting game...\n";
                break;
            default:
                std::cout << "Invalid option, try again.\n";
                break;
        }
    } while (choice != 5);
}

int main() {
    main_menu();
    return 0;
}
