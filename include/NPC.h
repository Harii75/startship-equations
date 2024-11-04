#ifndef NPC_H
#define NPC_H

#include <string>

struct NPC {
    int id;
    int stage;
    std::string name;
    std::string quest;
    std::string reward;
    std::string equation;
    std::string answer;
};

#endif 
