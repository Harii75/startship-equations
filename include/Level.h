#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include "Enemy.h"
#include "Reward.h"
#include "Condition.h"

class Level {
public:
    std::string name;
    std::string difficulty;
    std::vector<Enemy> enemies;
    Reward rewards;
    std::vector<Condition> conditions;
};

#endif
