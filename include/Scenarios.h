#ifndef SCENARIOS_H
#define SCENARIOS_H

#include "Player.h"
#include "Weapon.h"
#include "NPC.h"
#include "Challenges.h"
#include "Enemy.h"
#include <vector>

void randomScenario(Player& player, const std::vector<Weapon>& weapons, const std::vector<NPC>& npcs);

#endif