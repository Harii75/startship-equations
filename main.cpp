#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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
                std::cout << "- " << item.first << " (" << item.second << ")\n"; // Display item name and type
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
    int knowledge; // New knowledge point
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

// In-game menu options
void game_menu(Player& player) {
    int choice = 0;

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
            case 5: {
                std::cout << "Exploring...\n";
                int explore_outcome = rand() % 3; // Random number for exploration outcome
                player.knowledge += 10; // Instant knowledge point for exploring

                switch (explore_outcome) {
                    case 0:
                        std::cout << "You found nothing.\n";
                        break;
                    case 1:
                        std::cout << "You found free loot!\n";
                        player.inventory->add_item("Free Loot", "resource");
                        break;
                    case 2: {
                        std::cout << "Ship collides with an asteroid! Solve the equation to save the ship:\n";
                        // Generate a random equation
                        int a = rand() % 10 + 1;
                        int b = rand() % 10 + 1;
                        int answer = a + b; // Example: solve a + b

                        std::cout << "What is " << a << " + " << b << "? ";
                        int player_answer;
                        std::cin >> player_answer;

                        if (player_answer == answer) {
                            std::cout << "Correct! You saved the ship. +10 exploration knowledge.\n";
                            player.knowledge += 10; // Gain exploration knowledge
                        } else {
                            std::cout << "Incorrect! The ship takes damage.\n";
                            player.ship->health -= 50; // Ship takes damage
                            std::cout << "Ship health: " << player.ship->health << "\n";

                            if (player.ship->health <= 0) {
                                std::cout << "Game Over! Your ship has been destroyed.\n";
                                exit(0);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case 6:
                std::cout << "Exiting game...\n";
                break;
            default:
                std::cout << "Invalid option, try again.\n";
                break;
        }

    } while (choice != 6);
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation
    main_menu();
    return 0;
}
