#ifndef TRADE_MENU_H
#define TRADE_MENU_H

#include "Player.h"
#include "Buff.h"
#include <vector>

void tradeMenu(Player& player, const std::vector<Buff>& buffs);
int calculateSellPrice(const Weapon& weapon);

#endif
