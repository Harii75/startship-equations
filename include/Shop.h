#ifndef SHOP_H
#define SHOP_H

#include "Buff.h"
#include "Player.h"
#include <vector>
#include <utility>

extern std::vector<std::pair<Buff, int>> shopInventory;

int calculateBuffPrice(const Buff& buff);
void runShop(Player& player, const std::vector<Buff>& buffs);
void resetShopInventory(const std::vector<Buff>& buffs);

#endif
