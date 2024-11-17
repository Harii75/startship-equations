#include "GameProgression.h"
#include "Utils.h"
#include "Combat.h"
#include "Shop.h"
#include "HighScore.h"
#include "MainMenu.h"
#include <iostream>
#include <vector> 


void game_progression(Player& player, const std::vector<Stage>& stages, size_t& currentStage, size_t& currentLevel) {
    if (player.currentStage >= stages.size()) {
        std::cout << "No more stages available.\n";
        return;
    }

    for (size_t stageIndex = player.currentStage; stageIndex < stages.size(); ++stageIndex) {
        const auto& stage = stages[stageIndex];

        std::cout << "Entering Stage: " << stage.name << "\n";
        for (size_t levelIndex = player.currentLevel; levelIndex < stage.levels.size(); ++levelIndex) {
            const auto& level = stage.levels[levelIndex];

            // Check conditions before allowing the player to enter the level
            bool meetsConditions = true;
            for (const auto& condition : level.conditions) {
                if (player.knowledge < condition.condition_value) {
                    std::cout << "\nNot enough experience to access this level.\n";
                    std::cout << "Required Knowledge: " << condition.condition_value
                              << ", Your Knowledge: " << player.knowledge << "\n";
                    waitForKeypress();
                    clearScreen();
                    game_menu(player); // Redirect the player to the main menu
                    return;
                }
            }

            if (meetsConditions) {
                std::cout << "\nNow entering Level: " << level.name << " (" << level.difficulty << ")\n";

                // Fight each enemy in the level
                for (const auto& enemy : level.enemies) {
                    std::cout << "Encountered Enemy: " << enemy.type
                              << " - Health: " << enemy.health
                              << ", Damage: " << enemy.damage << "\n";
                    Enemy enemyCopy = enemy; // Make a copy to modify during the fight
                    fight(player, enemyCopy);
                }

                // Apply level rewards
                player.gold += level.rewards.gold;
                player.xp += level.rewards.xp;
                std::cout << "\nLevel Completed!\n";
                std::cout << "Rewards: " << level.rewards.gold << " gold, "
                          << level.rewards.xp << " XP\n";

                player.levelUp(player.xp); // Handle level-up logic

                // Ask the player whether to continue or return to the main menu
                char choice;
                std::cout << "\nChoose an option:\n";
                std::cout << "1. Continue to the next level\n";
                std::cout << "2. Return to the main menu\n\n";
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                waitForKeypress();
                clearScreen();

                if (choice == '2') {
                    player.currentStage = stageIndex;
                    player.currentLevel = levelIndex + 1; // Save progress at the next level
                    std::cout << "Returning to the main menu...\n";
                    player.visitCounter = 0;
                    game_menu(player); // Redirect to the main menu
                    return;
                }
            }
        }

        // Clear the current level when the stage is completed
        clearScreen();
        player.currentLevel = 0;
        std::cout << "Stage " << stage.name << " completed!\n";
        waitForKeypress();
    }

    // If all stages and levels are completed
    std::cout << "Congratulations! You've completed all stages!\n";
    waitForKeypress();
    addHighScore(player.name, player.highscore);
    finishGame(player);
}
