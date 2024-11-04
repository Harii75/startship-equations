#ifndef BUFF_H
#define BUFF_H

#include "Player.h"
#include <string>

struct Buff {
    int id;
    std::string name;
    int duration;
    std::string effect;
};

void applyBuff(Player& player, const Buff& buff);

#endif 
