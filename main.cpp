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
#include <cmath>

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

struct NPC {
    int id;
    int stage;
    std::string name;
    std::string quest;
    std::string reward;
    std::string equation;
    std::string answer;
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
    int upToNextLevel;
    int currentExpectedXP;
    int knowledge;
    ShipManager* ship;
    InventoryManager* inventory;
    size_t currentStage = 0;
    size_t currentLevel = 0;
    int highscore = 0;
    bool canTrade;

    Player(const std::string& name) : name(name), gold(500), level(1), xp(0), upToNextLevel(240), knowledge(0) {
        ship = new ShipManager();
        inventory = new InventoryManager();
    }

    void display_stats() {
        std::cout << "\n######################################################\n";
        std::cout << "                 Captain: " << name << "             \n";
        std::cout << "   Gold: " << gold << "\t\t\tLevel: " << level << "   XP: " << xp << "/" << upToNextLevel << "\n";
        std::cout << "   Damage: " << ship->damage <<  "\t\t\tHealth: " << ship->currentHealth <<"/" << ship->maxHealth << "\n";
        std::cout << "   Stage: [ " << currentStage+1 << " - " << currentLevel+1 << " ]\t\tKnowledge: " << knowledge << " \n";
        std::cout << "                    Score: " << highscore << " \n";
        std::cout << "######################################################\n";
    }

    int calculateDamage(int correctAnswer, int playerAnswer) {
        int difference = std::abs(correctAnswer - playerAnswer);
        if (difference == 0) {
            return ship->damage * 2;
        } else if (difference <= 2) {
            return ship->damage;
        } else if (difference <= 5) {
            return ship->damage / 2;
        } else {
            return 0;
        }
    }

    void gainXP(int points) {
        xp += points;
        std::cout << "You gained " << points << " XP!" << std::endl;

        //Check Level Up
        while(xp >= upToNextLevel){
            levelUp();
        }
    }

    void noCoutgainXP(int points) {
        xp += points;
        //Check Level Up
        while(xp >= upToNextLevel){
            levelUp();
        }   
    }

    void levelUp() {
        xp -= upToNextLevel;
        level++;
        upToNextLevel = static_cast<int>(upToNextLevel * 1.2);
        std::cout << "Congratulations! You've reached level " << level << "!\n";
    }


    void canPlayerTrade(){
        canTrade = (currentLevel % 5 == 0) ? true : false;
        std::cout << (canTrade ? "yes" : "no") << std::endl;
        std::cout << currentLevel;
    }

    ~Player() {
        delete ship;
        delete inventory;
    }
};

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

        const char* equation = npcElement->FirstChildElement("Equation")->GetText();
        if (equation) {
            npc.equation = equation;
        }

        const char* answer = npcElement->FirstChildElement("Answer")->GetText();
        if (answer) {
            npc.answer = answer;
        }

        npcs.push_back(npc);
    }

    return npcs; // Return the vector of NPCs
}

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
            tinyxml2::XMLElement* enemiesElement = levelElement->FirstChildElement("enemies");
            if (enemiesElement) {
                for (tinyxml2::XMLElement* enemyElement = enemiesElement->FirstChildElement("enemy"); enemyElement != nullptr; enemyElement = enemyElement->NextSiblingElement("enemy")) {
                    Enemy enemy;

                    // Retrieve type
                    const char* typeText = enemyElement->FirstChildElement("type")->GetText();
                    enemy.type = typeText ? typeText : "Unknown"; // Default if not found

                    // Retrieve health
                    tinyxml2::XMLElement* healthElement = enemyElement->FirstChildElement("health");
                    if (healthElement) {
                        enemy.health = healthElement->IntText();
                    } else {
                        enemy.health = -1; // Default if not found
                        std::cerr << "Warning: Health not found for enemy type: " << enemy.type << std::endl;
                    }

                    // Retrieve damage
                    tinyxml2::XMLElement* damageElement = enemyElement->FirstChildElement("damage");
                    if (damageElement) {
                        enemy.damage = damageElement->IntText();
                    } else {
                        enemy.damage = -1; // Default if not found
                        std::cerr << "Warning: Damage not found for enemy type: " << enemy.type << std::endl;
                    }

                    // Add enemy to level
                    level.enemies.push_back(enemy);
                }
            } else {
                std::cerr << "Warning: No enemies found in level: " << level.name << std::endl;
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

void printStages(const std::vector<Stage>& stages) {
    for (const auto& stage : stages) {
        std::cout << "Stage: " << stage.name << std::endl;
        for (const auto& level : stage.levels) {
            std::cout << "  Level: " << level.name << std::endl;
            for (const auto& enemy : level.enemies) {
                std::cout << "  Type: " << enemy.type << std::endl;
                std::cout << "  Health: " << enemy.health << std::endl;
                std::cout << "  Damage: " << enemy.damage << std::endl;
            }
        }
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
        std::cout << "Equation: " << npc.equation << std::endl;
        std::cout << "Answer: " << npc.answer << std::endl;
        std::cout << "---------------------------" << std::endl;
    }
    waitForKeypress();
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
                if (player.ship->maxHealth == player.ship->currentHealth){
                    std::cout << "Your ship is already fully repaired.";
                } else {
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

void generateEquation(int &num1, int &num2, char &operation, int &correctAnswer) {
    num1 = rand() % 20 + 1;
    num2 = rand() % 20 + 1;
    int op = rand() % 2;  // 0 for addition, 1 for subtraction

    if (op == 0) {
        operation = '+';
        correctAnswer = num1 + num2;
    } else {
        operation = '-';
        correctAnswer = num1 - num2;
    }
}

void fight(Player &player, Enemy &enemy) {

    while (player.ship->currentHealth > 0 && enemy.health > 0) {
        int num1, num2, playerAnswer, correctAnswer;
        char operation;

        generateEquation(num1, num2, operation, correctAnswer);

        // Display equation and get player's answer
        std::cout << "Solve this equation to attack: " << num1 << " " << operation << " " << num2 << " = ";
        std::cin >> playerAnswer;

        // Calculate damage based on accuracy
        int inflictedDamage = player.calculateDamage(correctAnswer, playerAnswer);
        enemy.health -= inflictedDamage;
        if (enemy.health < 0) {
            enemy.health = 0;
        }
        std::cout << "You dealt " << inflictedDamage << " damage to the enemy!" << std::endl;

        // Enemy's turn to attack
        if (enemy.health > 0) {
            int enemyDamage = rand() % 10 + 5;  // Random enemy damage
            player.ship->currentHealth -= enemyDamage;
            std::cout << "The enemy dealt " << enemyDamage << " damage to you!" << std::endl;
        }

        // Display health status
        std::cout << "Your health: " << player.ship->currentHealth << std::endl;
        std::cout << "Enemy health: " << enemy.health << std::endl;
        std::cout << "-----------------------------" << std::endl;
    }

    // Determine result of the fight
    if (player.ship->currentHealth > 0) {
        std::cout << "You defeated the enemy!" << std::endl;
        player.gainXP(50);
    } else {
        std::cout << "You were defeated by the enemy..." << std::endl;
    }
}

void asteroidCollisionChallenge(Player& player) {
    int goodAnswer = 0;
    std::cout << "\nYou are flying through an asteroid field, your left wing has hit an asteroid.\nComplete this 3 challange to stabilize you ship!\n";
    for (int i = 0; i < 3; ++i) {
        waitForKeypress();
        int num1 = rand() % 10 + 1;
        int num2 = rand() % 10 + 1;
        int correctAnswer = num1 * num2;

        std::cout << "\n\nYou have to recover you ship, solve this equation to steer: "
                  << num1 << " * " << num2 << " = ?" << std::endl;

        auto start = std::chrono::steady_clock::now();
        std::string answer;
        bool answeredInTime = false;

        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(5)) {
            if (_kbhit()) {
                char ch = _getch();
                if (ch == '\r') break;
                answer += ch;
                answeredInTime = true;
            }
        }


        if (!answeredInTime || answer.empty()) {
            std::cout << "\nYou could not avoid this asteroid.\n Part of your ship is badly damaged. (-30)!\n";
            player.ship->currentHealth -= 30;
        } else {
            int userAnswer = std::stoi(answer);
            if (userAnswer == correctAnswer) {
                goodAnswer += 1;
                std::cout << "Asteroid avoided! Stay focused!.\n";
            } else {
                std::cout << "Incorrect. The answer was: " << correctAnswer << ". You failed to steer your ship! (-15).\n";
                player.ship->currentHealth -= 20;
            }
        }

        if (player.ship->currentHealth <= 0) {
            std::cout << "Your couldn't avoid the asteroids. \nYour ship has fallen to pieces.\n";
            //endgame function
            break;
        }
    }


    switch(goodAnswer) {
        case 0:
            std::cout << "\nSomehow you made it. Unbelivable...";
            break;
        case 1:
            std::cout << "\nYou did you best, your left wing fully fell off.";
            break;
        case 2:
            std::cout << "\nWell done! Survived the asteroid collosion with minimal damage.";
            break;
        default:
            std::cout << "\nPerfect steering captain! You saved the day!";
            break;
    }
    std::cout << "\nChallenge complete. Player health: " << player.ship->currentHealth << std::endl;
    waitForKeypress();

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

            // Loop through levels starting from currentLevel for the current stage
            for (size_t levelIndex = player.currentLevel; levelIndex < stage.levels.size(); ++levelIndex) {
                const auto& level = stage.levels[levelIndex];
                std::cout << "\nNow in " << level.name << " (" << level.difficulty << ")\n";


                for (const Enemy& enemy : level.enemies) {
                    std::cout << "Encountered Enemy: " << enemy.type << " - Health: " << enemy.health << ", Damage: " << enemy.damage << "\n";

                    // Copy the enemy for the fight function if you need to modify it
                    Enemy enemyCopy = enemy; // Create a copy for the fight
                    fight(player, enemyCopy);
                }
                waitForKeypress();

                player.gold += level.rewards.gold;
                player.xp += level.rewards.xp;
                std::cout << "\nLevel Completed!\n";
                std::cout << "Rewards: " << level.rewards.gold << " gold, " << level.rewards.xp << " XP\n";

                waitForKeypress();
                clearScreen();

                char choice;
                std::cout << "Choose an option:";
                std::cout << "\n1. Continue to the next level\n";
                std::cout << "2. Return to the main menu\n";
                std::cout << "\nEnter your choice: ";
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
            clearScreen();
        }

        std::cout << "Congratulations! You've completed all stages!\n";
    }

NPC getRandomNpc(const std::vector<NPC>& npcs, int playerStage) {
    std::vector<NPC> eligibleNpcs;

    for (const NPC& npc : npcs) {
        if (npc.stage <= playerStage) {
            eligibleNpcs.push_back(npc);
        }
    }

    if (eligibleNpcs.empty()) {
        throw std::runtime_error("No eligible NPCs found for the player's current stage.");
    }

    // Select a random NPC from eligible list
    int randomIndex = rand() % eligibleNpcs.size();
    return eligibleNpcs[randomIndex];
}

Weapon findWeaponByRarity(const std::vector<Weapon>& weapons, const std::string& rarity) {
    for (const Weapon& weapon : weapons) {
        if (weapon.rarity == rarity) {
            return weapon;
        }
    }

    throw std::runtime_error("No weapon found with the specified rarity.");
}

void npcEquationChallange(Player& player, const NPC& npc, const std::vector<Weapon>& weapons) {
    std::cout << npc.quest << "\nEquation: " << npc.equation << std::endl;

    std::string playerAnswer;
    std::cout << "Enter your answer: ";
    std::cin >> playerAnswer;

    if (playerAnswer == npc.answer) {
        try {
            Weapon rewardWeapon = findWeaponByRarity(weapons, npc.reward);
            std::cout << "Correct! You've received a " << rewardWeapon.rarity << " weapon: "
                      << rewardWeapon.name << " with " << rewardWeapon.damage << " damage." << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } else {
        std::cout << "Incorrect answer. No reward this time." << std::endl;
    }
}

void randomScenario(Player& player, const std::vector<Weapon>& weapons) {
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
            asteroidCollisionChallenge(player);
            break;

        case 4: {
            std::cout << "You encounter an enemy ship!\n";
            Enemy darkShip;
            darkShip.damage = 10 * player.currentStage + 1;
            darkShip.health = (100 * player.currentStage + 1) / 2;
            darkShip.type = "Stealth Ship";
            fight(player, darkShip);
            player.knowledge += 5;
            break;
        }

        case 5: {
            std::cout << "Not implemented";
            //std::vector<Weapon> weapons = loadWeaponsFromXML("data/weapons.xml");
            //std::vector<NPC> npcs = loadNpcsFromXML("data/npc.xml");
            //std::cout << "You meet a mysterious NPC!\n";
            //NPC randomNPC = getRandomNpc(npcs, player.currentStage+1);
            //npcEquationChallange(player, randomNPC, weapons); // Pass weapons
            //player.knowledge += 5;
            break;
        }

        default:
            std::cout << "An unexpected event occurs in space.\n";
            break;
    }
}

void game_menu(Player& player) {
    int choice = 0;

    std::vector<Weapon> weapons = loadWeaponsFromXML("data/weapons.xml");
    std::vector<Stage> stages = loadStagesFromXML("data/levels.xml");
    std::vector<NPC> npcs = loadNpcsFromXML("data/npc.xml");

    player.inventory->add_random_items(weapons);

    waitForKeypress();
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
                std::cout << "The mission continues...\n";
                game_progression(player, stages, player.currentStage, player.currentLevel);
                waitForKeypress();
                break;
            case 2:
                clearScreen();
                std::cout << "Exploring the unknown..\n";
                randomScenario(player, weapons); // Pass weapons here
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
                player.canPlayerTrade();
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

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
    main_menu();
    return 0;
}
