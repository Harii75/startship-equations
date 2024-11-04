#ifndef GAME_PROGRESSION_H
#define GAME_PROGRESSION_H

#include "Player.h"
#include "Stage.h"
#include <vector>

void game_progression(Player& player, const std::vector<Stage>& stages, size_t& currentStage, size_t& currentLevel);

#endif