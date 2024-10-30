#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include "tinyxml2.h"
#include <ctime>
#include <random>
#include <chrono>
#include <memory>
#include <thread>

class Player;

void main_menu();

void game_menu(Player& player);

class Enemy {
public:
    std::string type;
    int health;
    int damage;
};

struct Weapon {
    std::string name;
    int damage;
    std::string rarity;
};

struct Buff {
    int id;
    std::string name;
    int duration;
    std::string effect;
};

struct NPC {
    int id;
    int stage;
    std::string name;
    std::string quest;
    std::string reward;
};

class Reward {
public:
    int gold;
    int xp;
    std::string special_item;
};

class Condition {
public:
    int condition_value;
};

class Level {
public:
    std::string name;
    std::string difficulty;
    std::vector<Enemy> enemies;
    Reward rewards;
    std::vector<Condition> conditions;
};

class Stage {
public:
    int id;
    std::string name;
    std::string description;
    std::vector<Level> levels;
};

class ShipManager {
public:
    int storage_capacity;
    int maxHealth;
    int currentHealth;
    int damage;

    ShipManager() : storage_capacity(100), currentHealth(100), damage(50), maxHealth(100) {}

    void upgrade_ship() {
        maxHealth += 25;
        currentHealth += 25;
        std::cout << "\n\nShip upgraded! New health: " << currentHealth << "/" << maxHealth << "\n";
    }

    void repair_ship() {
        currentHealth = maxHealth;
        std::cout << "\n\nShip fully repaired!\n";
    }

    void equip_weapon(int weapon_damage) {
        damage += weapon_damage;
        std::cout << "\n\nNew weapon equipped! Damage: " << damage << "\n";
    }

    void show_info() {
        std::cout << "Ship info:\n";
        std::cout << " - Capacity: " << storage_capacity << "\n";
        std::cout << " - Health: " << currentHealth << "/" << maxHealth <<"\n";
        std::cout << " - Damage: " << damage << "\n";
    }
};

class InventoryManager {
public:
    std::vector<std::pair<std::string, std::string>> items;

    InventoryManager() {}

    void add_item(const std::string& item, const std::string& type) {
        items.emplace_back(item, type);
        std::cout << item << " (" << type << ") added to inventory.\n";
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

    void add_random_items(const std::vector<Weapon>& weapons, int count = 2) {
        // Seed the generator with the current time
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
};

void waitForKeypress() {
    std::cout << "\nPress any button to continue...";
    _getch();
}
class Player {
public:
    std::string name;
    int gold;
    int level;
    int xp;
    int knowledge;
    ShipManager* ship;
    InventoryManager* inventory;
    size_t currentStage = 0;
    size_t currentLevel = 0;
    int highscore = 0;

    Player(const std::string& name) : name(name), gold(500), level(1), xp(0), knowledge(0) {
        ship = new ShipManager();
        inventory = new InventoryManager();
    }

    void display_stats() {
        std::cout << "\n######################################################\n";
        std::cout << "                 Captain: " << name << "             \n";
        std::cout << "   Gold: " << gold << "\t\t\tLevel: " << level << "   XP: " << xp << "/240  \n";
        std::cout << "   Damage: " << ship->damage <<  "\t\t\tHealth: " << ship->currentHealth <<"/" << ship->maxHealth << "\n";
        std::cout << "   Stage: [ " << currentStage+1 << " - " << currentLevel+1 << " ]\t\tKnowledge: " << knowledge << " \n";
        std::cout << "                    Score: " << highscore << " \n";
        std::cout << "######################################################\n";
    }

    //Levelup function kellene

    ~Player() {
        delete ship;
        delete inventory;
    }
};

std::vector<Weapon> loadWeaponsFromXML(const std::string& filePath) {
    std::vector<Weapon> weapons;
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return weapons; // Return empty vector on failure
    }

    tinyxml2::XMLElement* weaponsElement = doc.FirstChildElement("weapons");
    if (!weaponsElement) {
        std::cerr << "No <weapons> element found in the XML file." << std::endl;
        return weapons; // Return empty vector if no weapons element is found
    }

    for (tinyxml2::XMLElement* weaponElement = weaponsElement->FirstChildElement("weapon");
         weaponElement != nullptr; weaponElement = weaponElement->NextSiblingElement("weapon")) {

        Weapon weapon;

        const char* name = weaponElement->FirstChildElement("name")->GetText();
        if (name) {
            weapon.name = name;
        }

        weaponElement->FirstChildElement("damage")->QueryIntText(&weapon.damage);

        const char* rarity = weaponElement->FirstChildElement("rarity")->GetText();
        if (rarity) {
            weapon.rarity = rarity;
        }

        weapons.push_back(weapon);
    }

    return weapons; // Return the vector of weapons
}

std::vector<Buff> loadBuffsFromXML(const std::string& filePath) {
    std::vector<Buff> buffs;
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return buffs; // Return empty vector on failure
    }

    tinyxml2::XMLElement* buffsElement = doc.FirstChildElement("buffs");
    if (!buffsElement) {
        std::cerr << "No <buffs> element found in the XML file." << std::endl;
        return buffs; // Return empty vector if no buffs element is found
    }

    // Iterate through each <buff> element
    for (tinyxml2::XMLElement* buffElement = buffsElement->FirstChildElement("buff");
         buffElement != nullptr; buffElement = buffElement->NextSiblingElement("buff")) {

        Buff buff;
        buffElement->FirstChildElement("id")->QueryIntText(&buff.id);

        const char* name = buffElement->FirstChildElement("name")->GetText();
        if (name) {
            buff.name = name;
        }

        buffElement->FirstChildElement("duration")->QueryIntText(&buff.duration);

        const char* effect = buffElement->FirstChildElement("effect")->GetText();
        if (effect) {
            buff.effect = effect;
        }

        buffs.push_back(buff);
    }

    return buffs; // Return the vector of buffs
}

std::vector<Stage> loadStagesFromXML(const std::string& filePath) {
    std::vector<Stage> stages;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << filePath << std::endl;
        return stages;
    }

    // Navigate to the stages element
    tinyxml2::XMLElement* stagesElement = doc.FirstChildElement("game")->FirstChildElement("stages");
    for (tinyxml2::XMLElement* stageElement = stagesElement->FirstChildElement("stage"); stageElement != nullptr; stageElement = stageElement->NextSiblingElement("stage")) {
        Stage stage;
        stage.id = stageElement->IntAttribute("id");
        stage.name = stageElement->FirstChildElement("name")->GetText();
        stage.description = stageElement->FirstChildElement("description")->GetText();

        // Parse levels
        for (tinyxml2::XMLElement* levelElement = stageElement->FirstChildElement("levels")->FirstChildElement("level"); levelElement != nullptr; levelElement = levelElement->NextSiblingElement("level")) {
            Level level;
            level.name = levelElement->FirstChildElement("name")->GetText();
            level.difficulty = levelElement->FirstChildElement("difficulty")->GetText();

            // Parse enemies
            for (tinyxml2::XMLElement* enemyElement = levelElement->FirstChildElement("enemies")->FirstChildElement("enemy"); enemyElement != nullptr; enemyElement = enemyElement->NextSiblingElement("enemy")) {
                Enemy enemy;
                enemy.type = enemyElement->FirstChildElement("type")->GetText();
                enemy.health = enemyElement->IntAttribute("health");
                enemy.damage = enemyElement->IntAttribute("damage");
                level.enemies.push_back(enemy);
            }

            // Parse rewards
            level.rewards.gold = levelElement->FirstChildElement("rewards")->FirstChildElement("gold")->IntText();
            level.rewards.xp = levelElement->FirstChildElement("rewards")->FirstChildElement("xp")->IntText();
            if (levelElement->FirstChildElement("rewards")->FirstChildElement("special_item")) {
                level.rewards.special_item = levelElement->FirstChildElement("rewards")->FirstChildElement("special_item")->GetText();
            }

            // Parse conditions
            for (tinyxml2::XMLElement* conditionElement = levelElement->FirstChildElement("conditions")->FirstChildElement("condition"); conditionElement != nullptr; conditionElement = conditionElement->NextSiblingElement("condition")) {
                Condition condition;
                condition.condition_value = conditionElement->IntText();
                level.conditions.push_back(condition);
            }

            stage.levels.push_back(level);
        }

        stages.push_back(stage);
    }

    return stages;
}

std::vector<NPC> loadNpcsFromXML(const std::string& filePath) {
    std::vector<NPC> npcs;
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
        return npcs; // Return empty vector on failure
    }

    tinyxml2::XMLElement* npcsElement = doc.FirstChildElement("npcs");
    if (!npcsElement) {
        std::cerr << "No <npcs> element found in the XML file." << std::endl;
        return npcs; // Return empty vector if no npcs element is found
    }

    // Iterate through each <npc> element
    for (tinyxml2::XMLElement* npcElement = npcsElement->FirstChildElement("npc");
         npcElement != nullptr; npcElement = npcElement->NextSiblingElement("npc")) {

        NPC npc;

        npcElement->FirstChildElement("id")->QueryIntText(&npc.id);
        npcElement->FirstChildElement("stage")->QueryIntText(&npc.stage);

        const char* name = npcElement->FirstChildElement("name")->GetText();
        if (name) {
            npc.name = name;
        }

        const char* quest = npcElement->FirstChildElement("Quest")->GetText();
        if (quest) {
            npc.quest = quest;
        }

        const char* reward = npcElement->FirstChildElement("Reward")->GetText();
        if (reward) {
            npc.reward = reward;
        }

        npcs.push_back(npc);
    }

    return npcs; // Return the vector of NPCs
}

void printStages(const std::vector<Stage>& stages) {
    for (const auto& stage : stages) {
        std::cout << "Stage: " << stage.name << std::endl;
        for (const auto& level : stage.levels) {
            std::cout << "  Level: " << level.name << std::endl;
            for (const auto& enemy : level.enemies) {
                std::cout << "  Type: " << enemy.type << std::endl;
            }
        }
    }
}

void printBuffs(const std::vector<Buff>& buffs) {
    for (const auto& buff : buffs) {
        std::cout << "Buff ID: " << buff.id << std::endl;
        std::cout << "Name: " << buff.name << std::endl;
        std::cout << "Duration: " << buff.duration << std::endl;
        std::cout << "Effect: " << buff.effect << std::endl;
        std::cout << "--------------------" << std::endl;
    }
}

void printWeapons(const std::vector<Weapon>& weapons) {
    for (const auto& weapon : weapons) {
        std::cout << "Weapon Name: " << weapon.name << std::endl;
        std::cout << "Damage: " << weapon.damage << std::endl;
        std::cout << "Rarity: " << weapon.rarity << std::endl;
        std::cout << "--------------------" << std::endl;
    }
}

void printNpcs(const std::vector<NPC>& npcs) {
    for (const auto& npc : npcs) {
        std::cout << "NPC ID: " << npc.id << std::endl;
        std::cout << "Stage: " << npc.stage << std::endl;
        std::cout << "Name: " << npc.name << std::endl;
        std::cout << "Quest: " << npc.quest << std::endl;
        std::cout << "Reward: " << npc.reward << std::endl;
        std::cout << "--------------------" << std::endl;
    }
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void loadingEffect(int durationInSeconds, const std::string& message) {
    std::cout << message;
    for (int i = 0; i < durationInSeconds; ++i) {
        std::cout << ".";
        std::cout.flush();
        clock_t end_time = clock() + CLOCKS_PER_SEC;
        while (clock() < end_time) {}
    }
}

void equationChallenge() {
    int num1 = rand() % 10 + 1;
    int num2 = rand() % 10 + 1;
    int correctAnswer = num1 + num2;

    std::cout << "You have 5 seconds to answer this equation: " << num1 << " + " << num2 << " = ?" << std::endl;

    auto start = std::chrono::steady_clock::now();
    std::string answer;
    bool answeredInTime = false;

    // Give the user 5 seconds to answer
    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(5)) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '\r') break;  // Enter key
            answer += ch;
            answeredInTime = true;
        }
    }

    if (!answeredInTime) {
        std::cout << "\nTime's up! Now you can enter your answer: ";
        std::cin >> answer;
    }

    if (answer.empty()) {
        std::cout << "You're stupid.\n";
    } else {
        int userAnswer = std::stoi(answer);
        if (userAnswer == correctAnswer) {
            std::cout << "Correct! You were accurate.\n";
        } else {
            std::cout << "Incorrect. The correct answer was " << correctAnswer << ".\n";
        }
    }
}

void main_menu() {
    clearScreen();
    int choice = 0;
    do {
        clearScreen();
        std::cout << "\nMain Menu:\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Leaderboard\n";
        std::cout << "3. Exit\n";
        std::cout << "\nChoose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                clearScreen();
                std::string player_name;
                std::cout << "\nEnter your name: ";
                std::cin >> player_name;
                while (player_name.length() > 20){
                    std::cout << "\nName must be less than 20 characters. ";
                    std::cout << "\nEnter your name: ";
                    std::cin >> player_name;
                }
                Player player(player_name);
                game_menu(player);
                break;
            }
            case 2:
                clearScreen();
                std::cout << "Not available. Check Later\n";
                waitForKeypress();
                break;
            case 3:
                std::cout << "Exit...\n";
                break;
            default:
                clearScreen();
                std::cout << "Invalid input, try again.\n";
                break;
        }
    } while (choice != 3);
}

void ship_menu(Player& player) {
    int upgradeCost = 150;
    int repairCost = 100;
    int choice = 0;

    do {
        std::cout << "\nShip Menu:\n";
        std::cout << "1. Ship information \n";
        std::cout << "2. Upgrade ship\n";
        std::cout << "3. Repair ship\n";
        std::cout << "4. Equip weapon\n";
        std::cout << "5. Exit ship menu\n";
        std::cout << "\nChoose an option: ";
        std::cin >> choice;
        clearScreen();
        switch (choice) {
            case 1:
                std::cout << "Displaying ship information...\n\n";
                player.ship->show_info();
                std::cout << "\n";
                waitForKeypress();
                clearScreen();
                break;
            case 2: {
                std::cout << "Would you like to upgrade your ship? (150 gold) [Y/N]: ";
                std::string answer;
                std::cin >> answer;

                if (answer == "Y" || answer == "y") {
                    if (player.gold >= upgradeCost) {
                        player.gold -= upgradeCost;
                        loadingEffect(5,"\nUpgrading your ship");
                        player.ship->upgrade_ship();
                        std::cout << "Remaining gold: " << player.gold << "\n";
                    } else {
                        std::cout << "\nYou don't have enough gold (150 required).\n";
                    }
                }
                waitForKeypress();
                clearScreen();
                break;
            }

            case 3: {
                std::string answer;
                std::cout << "Would you like to repair your ship? (100 gold) [Y/N]: ";
                std::cin >> answer;
                if (answer == "Y" || answer == "y") {
                    if (player.gold >= repairCost) {
                        player.gold -= repairCost;
                        loadingEffect(5,"\nRepairing your ship");
                        player.ship->repair_ship();
                        std::cout << "Remaining gold: " << player.gold << "\n";
                    } else {
                        std::cout << "\nYou don't have enough gold (100 required).\n";
                    }
                }
                waitForKeypress();
                clearScreen();
                break;
            }

            case 4:
                break;

            case 5:
                std::cout << "Exiting...\n";
                clearScreen();
                break;

            default:
                std::cout << "Invalid choice, please try again.\n";
                clearScreen();
                break;
        }
    } while (choice != 5);
}

void randomScenario(Player& player) {
    player.knowledge += 5;
    std::srand(static_cast<unsigned int>(std::time(0)));

    int scenario = std::rand() % 5 + 1;

    switch (scenario) {
        case 1:
            std::cout << "Nothing happens as you drift through space.\n";
            break;

        case 2: {
            int freeGold = std::rand() % 101;
            player.gold += freeGold;
            std::cout << "You found a floating cabinet and collected " << freeGold << " gold!\n";
            break;
        }

        case 3:
            std::cout << "An asteroid collision! Solve these equations to stabilize your ship:\n";
            equationChallenge();
            break;

        case 4:
            std::cout << "You encounter an enemy ship! (Fight scenario skipped for now)\n";
            player.knowledge += 5;
            break;

        case 5:
            std::cout << "You meet a mysterious NPC! (NPC Interaction skipped for now)\n";
            player.knowledge += 5;
            break;

        default:
            std::cout << "An unexpected event occurs in space.\n";
            break;
    }
}

void game_progression(Player& player, const std::vector<Stage>& stages, size_t& currentStage, size_t& currentLevel) {
    // Check if the current stage is within bounds
    if (player.currentStage >= stages.size()) {
        std::cout << "No more stages available.\n";
        return;
    }

    // Loop through stages starting from currentStage
    for (size_t stageIndex = player.currentStage; stageIndex < stages.size(); ++stageIndex) {
        const auto& stage = stages[stageIndex];
        std::cout << "\nEntering Stage: " << stage.name << "\n" << stage.description << "\n";

        // Loop through levels starting from currentLevel for the current stage
        for (size_t levelIndex = player.currentLevel; levelIndex < stage.levels.size(); ++levelIndex) {
            const auto& level = stage.levels[levelIndex];
            std::cout << "\nNow in Level: " << level.name << " (" << level.difficulty << ")\n";

            for (const auto& enemy : level.enemies) {
                std::cout << "Encountered Enemy: " << enemy.type << " - Health: " << enemy.health << ", Damage: " << enemy.damage << "\n";
            }

            std::cout << "Player fights through the level...\n";
            waitForKeypress();

            player.gold += level.rewards.gold;
            player.xp += level.rewards.xp;
            std::cout << "\nLevel Completed!\n";
            std::cout << "Rewards: " << level.rewards.gold << " gold, " << level.rewards.xp << " XP\n";

            waitForKeypress();

            char choice;
            std::cout << "\nChoose an option:\n";
            std::cout << "1. Continue to the next level\n";
            std::cout << "2. Return to the main menu\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (choice == '2') {
                player.currentStage = stageIndex;
                player.currentLevel = levelIndex + 1;
                std::cout << "Returning to the main menu...\n";
                return;
            }
        }

        player.currentLevel = 0;
        std::cout << "Stage " << stage.name << " completed!\n";
        waitForKeypress();
    }

    std::cout << "Congratulations! You've completed all stages!\n";
}

void game_menu(Player& player) {
    int choice = 0;

    std::vector<Weapon> weapons = loadWeaponsFromXML("data/weapons.xml");
    std::vector<Stage> stages = loadStagesFromXML("data/levels.xml");
    std::vector<Buff> buffs = loadBuffsFromXML("data/buffs.xml");
    std::vector<NPC> npcs = loadNpcsFromXML("data/npc.xml");
    player.inventory->add_random_items(weapons);

    do {
        clearScreen();
        player.display_stats();
        std::cout << "\nGame Menu:\n";
        std::cout << "1. Continue Quest\n";
        std::cout << "2. Explore\n";
        std::cout << "3. Ship\n";
        std::cout << "4. Inventory\n";
        std::cout << "5. Trade\n";
        std::cout << "6. Exit\n";
        std::cout << "\nChoose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                clearScreen();
                std::cout<< "The mission continues...\n";
                game_progression(player,stages,player.currentStage,player.currentLevel);
                waitForKeypress();
                break;
            case 2:
                clearScreen();
                std::cout << "Exploring the unknown..\n";
                randomScenario(player);
                waitForKeypress();
                break;
            case 3:
                std::cout << "Displaying ship menu...\n";
                clearScreen();
                ship_menu(player);
                break;
            case 4:
                clearScreen();
                std::cout << "Displaying inventory...\n\n";
                player.inventory->show_inventory();
                waitForKeypress();
                break;
            case 5:
                clearScreen();
                std::cout << "You're not in a trading area.";
                waitForKeypress();
                break;
            case 6:
                clearScreen();
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
