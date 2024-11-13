#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "ShipManager.h"
#include "InventoryManager.h"

class Player {
public:
    Player(const std::string& name);
    ~Player();

    float accuracy = 1.0;
    int calculation_time = 30;
    int knowledge_points = 0;
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


    void display_stats();
    int calculateDamage(int correctAnswer, int playerAnswer);
    void levelUp(int& points);
    void canPlayerTrade();
    bool shouldResetShop();
};

#endif 
