#include "Player.h"
#include "Buff.h"
#include <iostream>

void applyBuff(Player& player, const Buff& buff) {
    if (buff.name == "Calculation Focus") {
        std::cout << "Buff applied: Calculation Focus - Increases accuracy of calculations by 10% for 3 rounds.\n";
    } else if (buff.name == "Quick Reflexes") {
        std::cout << "Buff applied: Quick Reflexes - Increases calculation accuracy seconds for 3 rounds.\n";
    } else if (buff.name == "Math Mastery") {
        std::cout << "Buff applied: Math Mastery - Boosts knowledge points gained from scenarios by 50% for 3 rounds.\n";
    } else if (buff.name == "Adaptive Learning") {
        std::cout << "Buff applied: Adaptive Learning - Reduces penalties for incorrect calculations for 3 rounds.\n";
    } else if (buff.name == "Enhanced Logic") {
        std::cout << "Buff applied: Enhanced Logic - Improves calculation efficiency, granting an additional 5 points for each correct answer for 3 rounds.\n";
    } else if (buff.name == "Mathematical Intuition") {
        std::cout << "Buff applied: Mathematical Intuition - Grants a hint for each calculation challenge, reducing difficulty by one level for 3 rounds.\n";
    } else if (buff.name == "Precision Calculation") {
        std::cout << "Buff applied: Precision Calculation - Boosts the accuracy of calculations in challenging encounters by 15% for 3 rounds.\n";
    } else if (buff.name == "Golden Ratio") {
        std::cout << "Buff applied: Golden Ratio - Increases gold rewards from calculations by 20% for 3 rounds.\n";
    } else if (buff.name == "Mental Fortitude") {
        std::cout << "Buff applied: Mental Fortitude - Restores a small amount of health after every three consecutive correct calculations for 3 rounds.\n";
    } else if (buff.name == "Astro Calculus") {
        std::cout << "Buff applied: Astro Calculus - Increases knowledge points gained per correct calculation by 5 points for 3 rounds.\n";
    } else if (buff.name == "Intuitive Math") {
        std::cout << "Buff applied: Intuitive Math - Allows partial credit for answers within a close range of the correct answer for 3 rounds.\n";
    } else {
        std::cout << "Buff applied: " << buff.name << " - No specific effect defined.\n";
    }
}
