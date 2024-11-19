#include "InteractNPC.h"
#include "InventoryManager.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>


NPC getRandomNPC(const std::vector<NPC>& npcs, size_t currentStage) {
    size_t currentStageCP = currentStage+1;
    std::vector<NPC> validNPCs;
    std::copy_if(npcs.begin(), npcs.end(), std::back_inserter(validNPCs),[currentStageCP](const NPC& npc) {
                     return npc.stage <= currentStageCP;
                 });

    if (validNPCs.empty()) {
        throw std::runtime_error("No NPCs available for the current stage.");
    }

    std::mt19937 gen(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<> dis(0, validNPCs.size() - 1);

    return validNPCs[dis(gen)];
}

// Function to interact with an NPC
void interactWithNPC(const std::vector<NPC>& npcs, const std::vector<Weapon>& weapons, 
                     InventoryManager& inventory, size_t currentStage) {
    try {
        NPC npc = getRandomNPC(npcs, currentStage);

        // Display NPC's challenge
        std::cout << "You encountered " << npc.name << "!" << std::endl;
        std::cout << "Quest: " << npc.quest << std::endl;
        std::cout << "Equation: " << npc.equation << std::endl;

        // Prompt the player for an answer
        std::string playerAnswer;
        std::cout << "\nYour answer: ";
        std::cin >> playerAnswer;

        // Check if the player's answer is correct
        if (playerAnswer == npc.answer) {            
            std::cout << "\nCorrect answer! You solved " << npc.name << "'s equation." << std::endl;

            try {
                std::vector<Weapon> filteredWeapons = inventory.filterWeaponsByRarity(weapons, npc.reward);
                Weapon rewardWeapon = inventory.getRandomWeapon(filteredWeapons);
                inventory.add_item(rewardWeapon);

                std::cout << "\nYou received a " << rewardWeapon.name << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "\nError: " << e.what() << std::endl;
            }
        } else {
            std::cout << "\nIncorrect answer. Better luck next time!" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
