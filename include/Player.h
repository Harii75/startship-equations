#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "ShipManager.h"
#include "InventoryManager.h"
#include "Buff.h"

class Player {
public:
    Player(const std::string& name);
    ~Player();

    float accuracy = 1.0;
    int calculation_time = 30;
    int lastShopResetLevel;
    std::string name;
    int gold;
    int level;
    int xp;
    int xpTreshold;
    int knowledge;
    ShipManager* ship;
    InventoryManager* inventory;
    size_t currentStage = 0;
    size_t currentLevel = 0;
    int highscore = 0;
    bool canTrade;
    int visitCounter = 0;
    std::vector<Buff> purchasedBuffs;

    void display_stats();
    int calculateDamage(int correctAnswer, int playerAnswer);
    void levelUp(int& points);
    void canPlayerTrade();
    bool shouldResetShop();
    void addPurchasedBuff(const Buff& buff) {
        purchasedBuffs.push_back(buff);
    };
    void listBuffs() const;
    void degradeBuffs();
};

#endif 
