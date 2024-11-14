#ifndef BUFF_H
#define BUFF_H

#include <string>

class Buff {
public:
    int id;  
    std::string name;
    std::string effect;
    int duration;

    Buff() : id(0), name(""), effect(""), duration(0) {}

    Buff(int buffId, const std::string& buffName, const std::string& buffEffect, int buffDuration)
        : id(buffId), name(buffName), effect(buffEffect), duration(buffDuration) {}
};

void applyBuff(class Player& player, const Buff& buff);

#endif 
