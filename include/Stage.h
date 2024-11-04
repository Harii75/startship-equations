#ifndef STAGE_H
#define STAGE_H

#include <string>
#include <vector>
#include "Level.h"

class Stage {
public:
    int id;
    std::string name;
    std::string description;
    std::vector<Level> levels;
};

#endif 
