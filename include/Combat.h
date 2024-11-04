#ifndef COMBAT_H
#define COMBAT_H

#include "Player.h"
#include "Enemy.h"

void generateEquation(int &num1, int &num2, char &operation, int &correctAnswer);
void fight(Player& player, Enemy& enemy);

#endif
