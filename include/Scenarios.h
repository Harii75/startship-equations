#ifndef SCENARIOS_H
#define SCENARIOS_H

#include "Player.h"
#include "Weapon.h"
#include "Challenges.h"
#include "Enemy.h"
#include <vector>

void randomScenario(Player& player, const std::vector<Weapon>& weapons);

#endif