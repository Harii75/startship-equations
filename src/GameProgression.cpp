#include "GameProgression.h"
#include "Utils.h"
#include "Combat.h"
#include <iostream>
#include "Shop.h"

void game_progression(Player& player, const std::vector<Stage>& stages, size_t& currentStage, size_t& currentLevel) {
    if (player.currentStage >= stages.size()) {
        std::cout << "No more stages available.\n";
        return;
    }

    for (size_t stageIndex = player.currentStage; stageIndex < stages.size(); ++stageIndex) {
        const auto& stage = stages[stageIndex];

        for (size_t levelIndex = player.currentLevel; levelIndex < stage.levels.size(); ++levelIndex) {
            const auto& level = stage.levels[levelIndex];
            std::cout << "\nNow in " << level.name << " (" << level.difficulty << ")\n";

            for (const Enemy& enemy : level.enemies) {
                std::cout << "Encountered Enemy: " << enemy.type << " - Health: " << enemy.health << ", Damage: " << enemy.damage << "\n";
                Enemy enemyCopy = enemy;
                fight(player, enemyCopy);
            }
            
            player.gold += level.rewards.gold;
            player.xp += level.rewards.xp;
            std::cout << "\nLevel Completed!\n";
            std::cout << "Rewards: " << level.rewards.gold << " gold, " << level.rewards.xp << " XP\n";

            player.levelUp(player.xp);


            char choice;
            std::cout << "Choose an option:\n";
            std::cout << "\n1. Continue to the next level\n";
            std::cout << "2. Return to the main menu\n";
            std::cout << "\nEnter your choice: ";
            std::cin >> choice;
            waitForKeypress();
            clearScreen();

            if (choice == '2') {
                player.currentStage = stageIndex;
                player.currentLevel = levelIndex + 1;
                clearScreen();
                std::cout << "Returning to the main menu...\n";
                player.visitCounter = 0;
                return;
            }
        }
        clearScreen();
        player.currentLevel = 0;
        std::cout << "Stage " << stage.name << " completed!\n";
        waitForKeypress();
    }

    std::cout << "Congratulations! You've completed all stages!\n";
}
