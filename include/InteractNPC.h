#ifndef INTERACTNPC_H
#define INTERACTNPC_H

#include "InventoryManager.h"
#include "NPC.h"
#include "Weapon.h"
#include <vector>
#include <string>

NPC getRandomNPC(const std::vector<NPC>& npcs, size_t currentStage);

void interactWithNPC(const std::vector<NPC>& npcs, const std::vector<Weapon>& weapons, 
                     InventoryManager& inventory, size_t currentStage);

#endif